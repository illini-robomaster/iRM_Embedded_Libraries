#include "motor.h"

uint8_t get_3508_data(motor_t* motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!can1_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!can2_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "CAN2_read failed");
                return 0;
            }
            break;
        default:
            bsp_error_handler(__FILE__, __LINE__, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m3508.angle[idx]       = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.m3508.speedRPM[idx]    = (int16_t)(buf[2] << 8 | buf[3]);
    motor->as.m3508.current_get      = (int16_t)(buf[4] << 8 | buf[5]);
    motor->as.m3508.temperature      = (uint8_t)buf[6];
    motor->cur_idx++;

    return 1;
}

void print_3508_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 3508 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %u\n", motor->as.m3508.angle[idx]);
    print("Current      %d\n", motor->as.m3508.current_get);
    print("Speed        %d\n", motor->as.m3508.speedRPM[idx]);
    print("Temperature  %u\n", motor->as.m3508.temperature);
    print("================================\n");
}

uint8_t get_6623_data(motor_t *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!can1_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!can2_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "can2_read failed");
                return 0;
            }
            break;
        default:
            bsp_error_handler(__FILE__, __LINE__, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m6623.angle[idx]    = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.m6623.current_get   = (int16_t)(buf[2] << 8 | buf[3]);
    motor->as.m6623.current_set   = (int16_t)(buf[4] << 8 | buf[5]);
    motor->cur_idx++;

    return 1;
}

void print_6623_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 6623 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %u\n", motor->as.m6623.angle[idx]);
    print("Current      %d\n", motor->as.m6623.current_get);
    print("Set Current  %d\n", motor->as.m6623.current_set);
    print("================================\n");
}

uint8_t get_3510_data(motor_t *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!can1_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!can2_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "CAN2_read failed");
                return 0;
            }
            break;
        default:
            bsp_error_handler(__FILE__, __LINE__, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m3510.angle[idx]  = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.m3510.current_get = (int16_t)(buf[2] << 8 | buf[3]);
    motor->cur_idx++;

    return 1;
}

void print_3510_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 3510 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %u\n", motor->as.m3510.angle[idx]);
    print("Current      %d\n", motor->as.m3510.current_get);
    print("================================\n");
}

uint8_t get_2006_data(motor_t *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (motor->can_id) {
        case CAN1_ID:
            if (!can1_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "CAN1_read failed");
                return 0;
            }
            break;
        case CAN2_ID:
            if (!can2_read(motor->sensor_id, buf)) {
                bsp_error_handler(__FILE__, __LINE__, "CAN2_read failed");
                return 0;
            }
            break;
        default:
            bsp_error_handler(__FILE__, __LINE__, "CAN ID doest not exist");
            return 0;
    }

    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m2006.angle[idx]      = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->as.m2006.speedRPM[idx]   = (int16_t)(buf[2] << 8 | buf[3]);
    motor->as.m2006.current_get     = (int16_t)(buf[4] << 8 | buf[5]);
    motor->cur_idx++;

    return 1;
}

void print_2006_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 2006 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %u\n", motor->as.m2006.angle[idx]);
    print("Current      %d\n", motor->as.m2006.current_get);
    print("Speed        %d\n", motor->as.m2006.speedRPM[idx]);
    print("================================\n");
}

void motor_id_init(motor_t *motor, uint16_t sensor_id, uint8_t can_id) {
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
