#include "motor.h"

/* private function starts from here */

void get_3508_data(motor_t* motor, uint8_t buf[CAN_DATA_SIZE]) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m3508.angle[idx]       = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_3508;
    motor->as.m3508.speedRPM[idx]    = \
        (int16_t)(buf[2] << 8 | buf[3]) * SPEED_CRT_3508;
    motor->as.m3508.current_get      = \
        (int16_t)(buf[4] << 8 | buf[5]) * CURRENT_CRT_3508;;
    motor->as.m3508.temperature      = \
        (uint8_t)buf[6];
    motor->cur_idx++;
}

void print_3508_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 3508 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %d\n", motor->as.m3508.angle[idx]);
    print("Current      %d\n", motor->as.m3508.current_get);
    print("Speed        %d\n", motor->as.m3508.speedRPM[idx]);
    print("Temperature  %u\n", motor->as.m3508.temperature);
    print("================================\n");
}

void get_6623_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m6623.angle[idx]    = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_6623;
    motor->as.m6623.current_get   = \
        (int16_t)(buf[2] << 8 | buf[3]) * CURRENT_CRT_6623;
    motor->as.m6623.current_set   = \
        (int16_t)(buf[4] << 8 | buf[5]) * CURRENT_CRT_6623;
    motor->cur_idx++;
}

void print_6623_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 6623 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %d\n", motor->as.m6623.angle[idx]);
    print("Current      %d\n", motor->as.m6623.current_get);
    print("Set Current  %d\n", motor->as.m6623.current_set);
    print("================================\n");
}

void get_3510_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m3510.angle[idx]  = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_3510;
    motor->as.m3510.current_get = \
        (int16_t)(buf[2] << 8 | buf[3]) * CURRENT_CRT_3510;
    motor->cur_idx++;
}

void print_3510_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 3510 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %d\n", motor->as.m3510.angle[idx]);
    print("Current      %d\n", motor->as.m3510.current_get);
    print("================================\n");
}

void get_2006_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    motor->as.m2006.angle[idx]      = \
        (int16_t)(buf[0] << 8 | buf[1]) * ANGLE_CRT_2006;
    motor->as.m2006.speedRPM[idx]   = \
        (int16_t)(buf[2] << 8 | buf[3]) * SPEED_CRT_2006;
    motor->as.m2006.current_get     = \
        (int16_t)(buf[4] << 8 | buf[5]) * CURRENT_CRT_2006;
    motor->cur_idx++;
}

void print_2006_data(motor_t* motor) {
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    print("== 2006 at CAN bus %u node %x ==\n", motor->can_id, motor->sensor_id);
    print("Angle        %d\n", motor->as.m2006.angle[idx]);
    print("Current      %d\n", motor->as.m2006.current_get);
    print("Speed        %d\n", motor->as.m2006.speedRPM[idx]);
    print("================================\n");
}

/* public function starts from here */

void motor_init(motor_t *motor, 
        uint16_t sensor_id, uint8_t can_id, motor_type_t type) {
    motor->type         = type;
    motor->can_id       = can_id;
    motor->sensor_id    = sensor_id;
    motor->cur_idx      = 0;
}

uint8_t get_motor_data(motor_t *motor) {
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
            bsp_error_handler(__FILE__, __LINE__, "CAN ID does not exist");
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
            bsp_error_handler(__FILE__, __LINE__, "motor type does not exist");
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
    uint8_t idx = motor->cur_idx % MAXIMUM_STATE;
    switch (motor->type) {
        case M3508:
            return motor->as.m3508.angle[idx];
        case M3510:
            return motor->as.m3510.angle[idx];
        case M6623:
            return motor->as.m6623.angle[idx];
        case M2006:
            return motor->as.m2006.angle[idx];
        default:
            bsp_error_handler(__FILE__, __LINE__, "motor type does not exist");
            return 0;
    }
}
