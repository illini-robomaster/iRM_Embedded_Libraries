#include "motor.h"
#include "bsp_can.h"

/* undocumented yet, do not use */
uint8_t get_chassis_prev_angle(motor_u *motor, uint8_t n) {
    int8_t idx = motor->chassis.cur_idx - n;
    if (idx < 0)
        idx += MAXIMUM_STATE;
    return motor->chassis.angle[idx];
}

/* undocumented yet, do not use */
uint8_t get_gimbal_prev_angle(motor_u *motor, uint8_t n) {
    int8_t idx = motor->gimbal.cur_idx - n;
    if (idx < 0)
        idx += MAXIMUM_STATE;
    return motor->gimbal.angle[idx];
}

uint8_t get_3508_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (can_id) {
        case CAN1_ID: 
            if (!CAN1_read(sensor_id, buf)) return 0;
            break;
        case CAN2_ID:
            if (!CAN2_read(sensor_id, buf)) return 0;
            break;
        default: return 0;
    }

    uint8_t idx = motor->extra_gimbal.cur_idx % MAXIMUM_STATE;
    motor->chassis.angle[idx]       = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->chassis.speedRPM[idx]    = (int16_t)(buf[2] << 8 | buf[3]);
    motor->chassis.current_get      = (int16_t)(buf[4] << 8 | buf[5]);
    motor->chassis.temperature      = (uint8_t)buf[6];
    motor->chassis.cur_idx++;

    return 1;
}

uint8_t get_6623_data(uint8_t can_id, uint8_t sensor_id, motor_u *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (can_id) {
        case CAN1_ID: 
            if (!CAN1_read(sensor_id, buf)) return 0;
            break;
        case CAN2_ID:
            if (!CAN2_read(sensor_id, buf)) return 0;
            break;
        default: return 0;
    }
    
    uint8_t idx = motor->extra_gimbal.cur_idx % MAXIMUM_STATE;
    motor->gimbal.angle[idx]    = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->gimbal.current_get   = (int16_t)(buf[2] << 8 | buf[3]);
    motor->gimbal.current_set   = (int16_t)(buf[4] << 8 | buf[5]);
    motor->gimbal.cur_idx++;

    return 1;
}

uint8_t get_3510_data(uint8_t can_id, uint8_t sensor_id, motor_u *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (can_id) {
        case CAN1_ID: 
            if (!CAN1_read(sensor_id, buf)) return 0;
            break;
        case CAN2_ID:
            if (!CAN2_read(sensor_id, buf)) return 0;
            break;
        default: return 0;
    }

    uint8_t idx = motor->extra_gimbal.cur_idx % MAXIMUM_STATE;
    motor->extra_gimbal.angle[idx]  = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->extra_gimbal.current_get = (int16_t)(buf[2] << 8 | buf[3]);
    motor->extra_gimbal.cur_idx++;

    return 1;
}

uint8_t get_2006_data(uint8_t can_id, uint8_t sensor_id, motor_u *motor) {
    uint8_t buf[CAN_DATA_SIZE];
    switch (can_id) {
        case CAN1_ID: 
            if (!CAN1_read(sensor_id, buf)) return 0;
            break;
        case CAN2_ID:
            if (!CAN2_read(sensor_id, buf)) return 0;
            break;
        default: return 0;
    }

    uint8_t idx = motor->extra_gimbal.cur_idx % MAXIMUM_STATE;
    motor->poke.angle[idx]      = (uint16_t)(buf[0] << 8 | buf[1]);
    motor->poke.speedRPM[idx]   = (int16_t)(buf[2] << 8 | buf[3]);
    motor->extra_gimbal.cur_idx++;

    return 1;
}


uint8_t get_chassis_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor) {
    return get_3508_data(can_id, sensor_id, motor);
}

uint8_t get_friction_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor) {
    return get_3508_data(can_id, sensor_id, motor);
}

uint8_t get_gimbal_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor) {
    return get_6623_data(can_id, sensor_id, motor);
}

uint8_t get_extra_gimbal_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor) {
    return get_3510_data(can_id, sensor_id, motor);
}

uint8_t get_poke_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor) {
    return get_2006_data(can_id, sensor_id, motor);
}

