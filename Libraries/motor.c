/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "motor.h"
#include "utils.h"
#include <stdlib.h>

/* private function starts from here */

static void get_3508_data(motor_t* motor, uint8_t buf[CAN_DATA_SIZE]) {
    motor->as.m3508.angle            = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_3508;
    motor->as.m3508.speed_rpm        = \
        (int16_t)(buf[2] << 8 | buf[3]) * SPEED_CRT_3508;
    motor->as.m3508.current_get      = \
        (int16_t)(buf[4] << 8 | buf[5]) * CURRENT_CRT_3508;;
    motor->as.m3508.temperature      = \
        (uint8_t)buf[6];
}

static void print_3508_data(motor_t* motor) {
    print("== 3508 at CAN bus %u node %x ==\r\n", motor->as.mdjican.can_id, motor->as.mdjican.rx_id);
    print("Angle        %d\n", motor->as.m3508.angle);
    print("Current      %d\n", motor->as.m3508.current_get);
    print("Speed        %d\n", motor->as.m3508.speed_rpm);
    print("Temperature  %u\n", motor->as.m3508.temperature);
    print("================================\r\n");
}

static void get_6623_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]) {
    motor->as.m6623.angle         = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_6623;
    motor->as.m6623.current_get   = \
        (int16_t)(buf[2] << 8 | buf[3]) * CURRENT_CRT_6623;
    motor->as.m6623.current_set   = \
        (int16_t)(buf[4] << 8 | buf[5]) * CURRENT_CRT_6623;
}

static void print_6623_data(motor_t* motor) {
    print("== 6623 at CAN bus %u node %x ==\r\n", motor->as.mdjican.can_id, motor->as.mdjican.rx_id);
    print("Angle        %d\r\n", motor->as.m6623.angle);
    print("Current      %d\r\n", motor->as.m6623.current_get);
    print("Set Current  %d\r\n", motor->as.m6623.current_set);
    print("================================\r\n");
}

static void get_3510_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]) {
    motor->as.m3510.angle       = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_3510;
    motor->as.m3510.current_get = \
        (int16_t)(buf[2] << 8 | buf[3]) * CURRENT_CRT_3510;
}

static void print_3510_data(motor_t* motor) {
    print("== 3510 at CAN bus %u node %x ==\n", motor->as.mdjican.can_id, motor->as.mdjican.rx_id);
    print("Angle        %d\n", motor->as.m3510.angle);
    print("Current      %d\n", motor->as.m3510.current_get);
    print("================================\n");
}

static void get_2006_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]) {
    motor->as.m2006.angle           = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_2006;
    motor->as.m2006.speed_rpm       = \
        (int16_t)(buf[2] << 8 | buf[3]) * SPEED_CRT_2006;
    motor->as.m2006.current_get     = \
        (int16_t)(buf[4] << 8 | buf[5]) * CURRENT_CRT_2006;
}

static void print_2006_data(motor_t* motor) {
    print("== 2006 at CAN bus %u node %x ==\n", motor->as.mdjican.can_id, motor->as.mdjican.rx_id);
    print("Angle        %d\n", motor->as.m2006.angle);
    print("Current      %d\n", motor->as.m2006.current_get);
    print("Speed        %d\n", motor->as.m2006.speed_rpm);
    print("================================\n");
}

static uint8_t match_id(uint16_t *old_id, uint16_t new_id) {
    if (!*old_id) {
        *old_id = new_id;
        return 1;
    }
    else
        return (*old_id == new_id);
}

int16_t clip(int16_t val, int16_t range) {
    if (val >= range / 2)
        val -= range;
    else if (val <= -range / 2)
        val += range;
    return val;
}

static int16_t current_limit(float val, int16_t lim) {
    if (val < -lim)
        return -lim;
    if (val > lim)
        return lim;
    return (int16_t)val;
}

static int16_t correct_output(motor_t *motor) {
    switch (motor->type) {
        case M3508:
            return current_limit(motor->out * CURRENT_CRT_3508,
                    CURRENT_MAX_3508);
        case M3510:
            return current_limit(motor->out * CURRENT_CRT_3510,
                    CURRENT_MAX_3510);
        case M6623:
            return current_limit(motor->out * CURRENT_CRT_6623,
                    CURRENT_MAX_6623);
        case M2006:
            return current_limit(motor->out * CURRENT_CRT_2006,
                    CURRENT_MAX_2006);
        case M2305:
            return fclip_to_range(&motor->out, CURRENT_MIN_2305, CURRENT_MAX_2305);
        case MPWM:
            return motor->out;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "motor type undefined");
            return 0;
    }
}

/* public function starts from here */

motor_t *can_motor_init(motor_t *motor,
        uint16_t rx_id, uint8_t can_id, motor_type_t type) {
    if (!motor)
        motor = pvPortMalloc(sizeof(motor_t));
    motor->type                 = type;
    motor->as.mdjican.can_id    = can_id;
    motor->as.mdjican.rx_id                = rx_id;
    motor->out                  = 1;
    motor->target               = 0;
    if (rx_id >= CAN_RX1_START &&
            rx_id < CAN_RX1_START + CAN_GROUP_SIZE)
        motor->as.mdjican.tx_id = CAN_TX1_ID;
    else if (rx_id >= CAN_RX2_START &&
            rx_id < CAN_RX2_START + CAN_GROUP_SIZE)
        motor->as.mdjican.tx_id = CAN_TX2_ID;
    else if (rx_id >= CAN_RX3_START &&
            rx_id < CAN_RX3_START + CAN_GROUP_SIZE)
        motor->as.mdjican.tx_id = CAN_TX3_ID;
    else
        bsp_error_handler(__FUNCTION__, __LINE__, "rx id out of range");
    /* transmit non-zero data to can bus to avoid initial motor burst */
    for (size_t i = 0; i < 30; i++)
        set_can_motor_output(motor, motor, motor, motor);
    return motor;
}

motor_t *pwm_motor_init(motor_t *motor, motor_type_t type,
        pwm_t *pwm, uint32_t idle_throttle) {
    if (!motor)
        motor = pvPortMalloc(sizeof(motor_t));

    motor->type     = type;
    motor->out      = 0;
    motor->target   = 0;

    motor->as.mpwm.pwm              = pwm;
    motor->as.mpwm.idle_throttle    = idle_throttle;

    return motor;
}

uint8_t get_motor_data(motor_t *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->as.mdjican.can_id) {
        case CAN1_ID:
            if (!can1_read(motor->as.mdjican.rx_id, buf)) {
                bsp_error_handler(__FUNCTION__, __LINE__, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!can2_read(motor->as.mdjican.rx_id, buf)) {
                bsp_error_handler(__FUNCTION__, __LINE__, "can2_read failed");
                return 0;
            }
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "CAN ID does not exist");
            return 0;
    }
    switch (motor->type) {
        case M3508:
            get_3508_data(motor, buf);
            return 1;
        case M3510:
            get_3510_data(motor, buf);
            return 1;
        case M6623:
            get_6623_data(motor, buf);
            return 1;
        case M2006:
            get_2006_data(motor, buf);
            return 1;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "motor type does not exist");
            return 0;
    }
}

void print_motor_data(motor_t *motor) {
    switch (motor->type) {
        case M3508:
            print_3508_data(motor);
            return;
        case M3510:
            print_3510_data(motor);
            return;
        case M6623:
            print_6623_data(motor);
            return;
        case M2006:
            print_2006_data(motor);
            return;
        default: return;
    }
}

int16_t get_motor_angle(motor_t *motor) {
    switch (motor->type) {
        case M3508:
        case M3510:
        case M6623:
        case M2006:
            return motor->as.mdjican.angle;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "motor type does not support angle attribute");
            return 0;
    }
}

int16_t get_angle_err(motor_t *motor, int16_t target) {
    int16_t diff;
    switch (motor->type) {
        case M3508:
        case M3510:
        case M6623:
        case M2006:
            diff = target - motor->as.mdjican.angle;
            return clip(diff, ANGLE_RANGE_DJI);
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "motor type does not support angle attribute");
            return 0;
    }
}

int16_t clip_angle_err(motor_t *motor, int16_t err) {
    switch (motor->type) {
        case M3508:
        case M3510:
        case M6623:
        case M2006:
            return clip(err, ANGLE_RANGE_DJI);
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "motor type does not support angle attribute");
            return 0;
    }
}

int16_t get_speed_err(motor_t *motor, int16_t target) {
    switch (motor->type) {
        case M3508:
            return target - motor->as.m3508.speed_rpm;
        case M2006:
            return target - motor->as.m2006.speed_rpm;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "motor type does not support speed attribute");
            return 0;
    }
}

uint8_t set_can_motor_output(motor_t *motor1, motor_t *motor2,
        motor_t *motor3, motor_t *motor4) {
    int16_t sp1, sp2, sp3, sp4;
    uint16_t tx_id, can_id;
    sp1 = sp2 = sp3 = sp4 = 0;
    tx_id = can_id = 0;

    if ((motor1 && (!match_id(&tx_id, motor1->as.mdjican.tx_id) || !match_id(&can_id, motor1->as.mdjican.can_id))) ||
        (motor2 && (!match_id(&tx_id, motor2->as.mdjican.tx_id) || !match_id(&can_id, motor2->as.mdjican.can_id))) ||
        (motor3 && (!match_id(&tx_id, motor3->as.mdjican.tx_id) || !match_id(&can_id, motor3->as.mdjican.can_id))) ||
        (motor4 && (!match_id(&tx_id, motor4->as.mdjican.tx_id) || !match_id(&can_id, motor4->as.mdjican.can_id)))) {
        bsp_error_handler(__FUNCTION__, __LINE__, "motor can / tx id not matched");
        return 0;
    }

    if (motor1) { sp1 = correct_output(motor1); }
    if (motor2) { sp2 = correct_output(motor2); }
    if (motor3) { sp3 = correct_output(motor3); }
    if (motor4) { sp4 = correct_output(motor4); }

    switch (can_id) {
        case CAN1_ID:
            can1_transmit(tx_id, sp1, sp2, sp3, sp4);
            break;
        case CAN2_ID:
            can2_transmit(tx_id, sp1, sp2, sp3, sp4);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "can id does not exist");
            return 0;
    }

    return 1;
}

void set_pwm_motor_output(motor_t *motor) {
    uint32_t output = correct_output(motor);

    output += motor->as.mpwm.idle_throttle;
    pwm_set_pulse_width(motor->as.mpwm.pwm, output);
}
