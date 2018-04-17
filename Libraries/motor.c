#include "motor.h"
#include "bsp_error_handler.h"

uint8_t get_3508_data(motor_t* motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!CAN1_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 9, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!CAN2_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 15, "CAN2_read failed");
                return 0;
            }
            break;
        default: 
            bsp_error_handler("motor.c", 20, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.chassis.angle[idx]       = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.chassis.speedRPM[idx]    = (int16_t)(buf[2] << 8 | buf[3]);
    motor->as.chassis.current_get      = (int16_t)(buf[4] << 8 | buf[5]);
    motor->as.chassis.temperature      = (uint8_t)buf[6];
    motor->cur_idx++;

    return 1;
}

uint8_t get_6623_data(motor_t *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!CAN1_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 39, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!CAN2_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 45, "CAN2_read failed");
                return 0;
            }
            break;
        default: 
            bsp_error_handler("motor.c", 50, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.gimbal.angle[idx]    = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.gimbal.current_get   = (int16_t)(buf[2] << 8 | buf[3]);
    motor->as.gimbal.current_set   = (int16_t)(buf[4] << 8 | buf[5]);
    motor->cur_idx++;

    return 1;
}

uint8_t get_3510_data(motor_t *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!CAN1_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 68, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!CAN2_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 74, "CAN2_read failed");
                return 0;
            }
            break;
        default: 
            bsp_error_handler("motor.c", 79, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.extra_gimbal.angle[idx]  = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.extra_gimbal.current_get = (int16_t)(buf[2] << 8 | buf[3]);
    motor->cur_idx++;

    return 1;
}

uint8_t get_2006_data(motor_t *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!CAN1_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 96, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!CAN2_read(motor->sensor_id, buf)) {
                bsp_error_handler("motor.c", 102, "CAN2_read failed");
                return 0;
            }
            break;
        default: 
            bsp_error_handler("motor.c", 107, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.poke.angle[idx]      = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.poke.speedRPM[idx]   = (int16_t)(buf[2] << 8 | buf[3]);
    motor->cur_idx++;

    return 1;
}

void motor_id_init(motor_t *motor, uint8_t sensor_id, uint8_t can_id) {
    motor->can_id       = can_id;
    motor->sensor_id    = sensor_id;
    motor->cur_idx      = 0;
}

uint8_t get_chassis_data(motor_t* motor) {
    return get_3508_data(motor);
}

uint8_t get_friction_data(motor_t* motor) {
    return get_3508_data(motor);
}

uint8_t get_gimbal_data(motor_t* motor) {
    return get_6623_data(motor);
}

uint8_t get_extra_gimbal_data(motor_t* motor) {
    return get_3510_data(motor);
}

uint8_t get_poke_data(motor_t* motor) {
    return get_2006_data(motor);
}
