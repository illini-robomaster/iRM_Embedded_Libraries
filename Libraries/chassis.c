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

#include "chassis.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>

#ifdef INFANTRY1

static pid_ctl_t chassis_rotate;
const static int16_t evasive_tar[2] = {EVASIVE_LEFTMOST_YAW, EVASIVE_RIGHTMOST_YAW};
static uint8_t evasive_tar_cnt = 0;

/**
 * backward:    -1
 * disabled:    0
 * forward:     1
 */
int8_t direction = 1;

void chassis_init(pid_ctl_t *my_chassis[4]){
    motor_t *m_fl, *m_fr, *m_rl, *m_rr;
    pid_ctl_t *pid_fl, *pid_fr, *pid_rl, *pid_rr;
    m_fl = m_fr = m_rl = m_rr = NULL;
    pid_fl = pid_fr = pid_rl = pid_rr = NULL;

    m_fl = can_motor_init(NULL, FL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_fr = can_motor_init(NULL, FR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rl = can_motor_init(NULL, RL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rr = can_motor_init(NULL, RR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);

    pid_fl = pid_init(NULL, CHASSIS_ROTATE, m_fl, -MAX_SPEED, MAX_SPEED, CHS_INT_LIM,
                0, 0, CHS_KP, CHS_KI, CHS_KD, 0, 0);
    pid_fr = pid_init(NULL, CHASSIS_ROTATE, m_fr, -MAX_SPEED, MAX_SPEED, CHS_INT_LIM,
                0, 0, CHS_KP, CHS_KI, CHS_KD, 0, 0);
    pid_rl = pid_init(NULL, CHASSIS_ROTATE, m_rl, -MAX_SPEED, MAX_SPEED, CHS_INT_LIM,
                0, 0, CHS_KP, CHS_KI, CHS_KD, 0, 0);
    pid_rr = pid_init(NULL, CHASSIS_ROTATE, m_rr, -MAX_SPEED, MAX_SPEED, CHS_INT_LIM,
                0, 0, CHS_KP, CHS_KI, CHS_KD, 0, 0);

    my_chassis[CHASSIS_FL] = pid_fl;
    my_chassis[CHASSIS_FR] = pid_fr;
    my_chassis[CHASSIS_RL] = pid_rl;
    my_chassis[CHASSIS_RR] = pid_rr;

    pid_init(&chassis_rotate, MANUAL_ERR_INPUT, m_fl, 0, 0,
                0, 0, 0, ROTATE_KP, 0, 0, MAX_TURN_SPEED, YAW_DEADBAND);
}

void calc_keyboard_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle, float *prev_vx, float *prev_vy) {
    // counterclockwise: positive
    yaw_angle = -yaw_angle + Q_PI;
    float v_y = 0;
    float v_x = 0;

    if (rc->key.bit.W) v_y += 1;
    if (rc->key.bit.S) v_y -= 1;
    if (rc->key.bit.A) v_x -= 1;
    if (rc->key.bit.D) v_x += 1;
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    if (sign(v_y) * sign(*prev_vy) == -1)
        v_y = 0;
    /*
    else if (fabs(v_y) > 0.2 && fabs(*prev_vy) < 0.2)
        v_y = sign(v_y) * 0.2;
        */
    else if (fabs(v_y) - fabs(*prev_vy) >= 0.01) {
        if (referee_info.power_heat_data.chassis_power <= 50)
            v_y = *prev_vy + sign(v_y - *prev_vy) * 0.01;
        else if (referee_info.power_heat_data.chassis_power >= 70)
            v_y = *prev_vy - sign(v_y - *prev_vy) * 0.01;
        else
            v_y = *prev_vy;
    }

    if (sign(v_x) * sign(*prev_vx) == -1)
        v_x = 0;
    /*
    else if (fabs(v_x) > 0.1 && fabs(*prev_vx) < 0.1)
        v_x = sign(v_x) * 0.1;
        */
    else if (fabs(v_x) - fabs(*prev_vx) >= 0.007) {
        if (referee_info.power_heat_data.chassis_power <= 50)
            v_x = *prev_vx + sign(v_x - *prev_vx) * 0.007;
        else if (referee_info.power_heat_data.chassis_power >= 70)
            v_x = *prev_vx - sign(v_x - *prev_vx) * 0.007;
        else
            v_x = *prev_vx;
    }
#endif

    // rotation; change of basis matrix.
    float target_x = (v_x * cos(yaw_angle) + v_y * sin(yaw_angle)) * MAX_LINEAR_SPEED;
    float target_y = (-v_x * sin(yaw_angle) + v_y * cos(yaw_angle)) * MAX_LINEAR_SPEED;
    my_chassis[CHASSIS_FL]->motor->target = direction * target_x;
    my_chassis[CHASSIS_RR]->motor->target = direction * (-target_x); // velocity is the same. It's just these two motors are installed in opposite direction.
    my_chassis[CHASSIS_RL]->motor->target = direction * target_y;
    my_chassis[CHASSIS_FR]->motor->target = direction * (-target_y);
    *prev_vx = v_x;
    *prev_vy = v_y;
}

void calc_remote_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle, float *prev_vx, float *prev_vy) {
    yaw_angle = -yaw_angle + Q_PI;
    float v_y = rc->ch1 * 1.0 / 660;
    float v_x = rc->ch0 * 1.0 / 660;
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    if (sign(v_y) * sign(*prev_vy) == -1)
        v_y = 0;
    else if (fabs(v_y) - fabs(*prev_vy) >= 0.007) {
        if (referee_info.power_heat_data.chassis_power <= 45)
            v_y = *prev_vy + sign(v_y - *prev_vy) * 0.007;
        else if (referee_info.power_heat_data.chassis_power >= 70)
            v_y = *prev_vy - sign(v_y - *prev_vy) * 0.007;
        else
            v_y = *prev_vy;
    }

    if (sign(v_x) * sign(*prev_vx) == -1)
        v_x = 0;
    else if (fabs(v_x) - fabs(*prev_vx) >= 0.007) {
        if (referee_info.power_heat_data.chassis_power <= 45)
            v_x = *prev_vx + sign(v_x - *prev_vx) * 0.007;
        else if (referee_info.power_heat_data.chassis_power >= 70)
            v_x = *prev_vx - sign(v_x - *prev_vx) * 0.007;
        else
            v_x = *prev_vx;
    }
#endif
    // rotation; change of basis matrix.
    float target_x = (v_x * cos(yaw_angle) + v_y * sin(yaw_angle)) * MAX_LINEAR_SPEED;
    float target_y = (-v_x * sin(yaw_angle) + v_y * cos(yaw_angle)) * MAX_LINEAR_SPEED;
    my_chassis[CHASSIS_FL]->motor->target = direction * target_x;
    my_chassis[CHASSIS_RR]->motor->target = direction * (-target_x); // velocity is the same. It's just these two motors are installed in opposite direction.
    my_chassis[CHASSIS_RL]->motor->target = direction * target_y;
    my_chassis[CHASSIS_FR]->motor->target = direction * (-target_y);
    *prev_vx = v_x;
    *prev_vy = v_y;
}

void evasive_move(pid_ctl_t *my_chassis[4], int16_t cur_yaw_feedback, motor_t *yaw_motor) {
    if(abs(cur_yaw_feedback - EVASIVE_LEFTMOST_YAW) < EVASIVE_DEADBAND) {
        evasive_tar_cnt = 1;
    }
    if(abs(cur_yaw_feedback - EVASIVE_RIGHTMOST_YAW) < EVASIVE_DEADBAND) {
        evasive_tar_cnt = 0;
    }

    /* adjust maximum turning speed for evasive mode */
    chassis_rotate.maxout = EVASIVE_TURN_SPEED;
    adjust_chassis_gimbal_pos(my_chassis, evasive_tar[evasive_tar_cnt], yaw_motor);
    chassis_rotate.maxout = MAX_TURN_SPEED;
}

void adjust_chassis_gimbal_pos(pid_ctl_t *my_chassis[4], int16_t desired_yaw_angle, motor_t *yaw_motor) {
    int16_t deviation = get_angle_err(yaw_motor, desired_yaw_angle);
    int32_t rotate_speed = pid_calc(&chassis_rotate, deviation);

    for (int i = 0; i < 4; ++i)
        my_chassis[i]->motor->target += abs(direction) * rotate_speed;
}

void chassis_mode_forward(void) {
    direction = 1;
}

void chassis_mode_backward(void) {
    direction = -1;
}

void chassis_stop(void) {
    direction = 0;
}

void chassis_kill(pid_ctl_t *my_chassis[4]) {
    for (uint8_t i = 0; i < 4; ++i)
        my_chassis[i]->motor->out = 0;

    while (1) {
        set_can_motor_output(my_chassis[CHASSIS_FL]->motor, my_chassis[CHASSIS_FR]->motor,
                    my_chassis[CHASSIS_RL]->motor, my_chassis[CHASSIS_RR]->motor);
        osDelay(1000);
    }
}

void run_chassis(pid_ctl_t *my_chassis[4]){
    for (uint8_t i = 0; i < 4; ++i)
        my_chassis[i]->motor->out = pid_calc(my_chassis[i], my_chassis[i]->motor->target);

    set_can_motor_output(my_chassis[CHASSIS_FL]->motor, my_chassis[CHASSIS_FR]->motor,
                my_chassis[CHASSIS_RL]->motor, my_chassis[CHASSIS_RR]->motor);
}

#endif
