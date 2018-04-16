/**
 * @author  Alvin Sun
 * @data    2018-04-16
 * @file    motor.h
 * @brief   library level motor data processing
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <inttypes.h>
#include "bsp_can.h"

/**
 * @ingroup library
 * @defgroup motor Motor
 * @{
 */

#define CAN1_ID 1
#define CAN2_ID 2

#define MAXIMUM_STATE 4

#define ANGLE_MIN_3510      0       // 0    degree
#define ANGLE_MAX_3510      8191    // 360  degree
#define CURRENT_MIN_3510    UNKNOWN
#define CURRENT_MAX_3510    UNKNOWN

#define ANGLE_MIN_3508      0       // 0    degree
#define ANGLE_MAX_3508      8191    // 360  degree
#define CURRENT_MIN_3508    -16384
#define CURRENT_MAX_3508    16384

#define ANGLE_MIN_6623      0       // 0    degree
#define ANGLE_MAX_6623      8191    // 360  degree
#define CURRENT_MIN_6623    -13000
#define CURRENT_MAX_6623    13000

#define ANGLE_MIN_2006      0       // 0    degree
#define ANGLE_MAX_2006      8191    // 360  degree
#define CURRENT_MIN_2006    -10000
#define CURRENT_MAX_2006    10000

/**
 * @struct  motor_3508_t
 * @brief   store 3508 motor data
 * @var cur_idx     current index to write into the buffer
 * @var angle       a circular buffer to store current and previous angle
 * @var current_get actual current / torque output
 * @var speedRPM    rotational speed in RPM (Rotation Per Minute)
 * @var temperature sensor temperature in celcius degree
 */
typedef struct {
    uint8_t     cur_idx;
    uint16_t    angle[MAXIMUM_STATE];
    int16_t     current_get;
    int16_t     speedRPM[MAXIMUM_STATE];
    uint8_t     temperature;
}   motor_3508_t;

/**
 * @struct  motor_6623_t
 * @brief   store 6623 motor data
 * @var cur_idx     current index to write into the buffer
 * @var angle       a circular buffer to store current and previous angle
 * @var current_get actual current / torque output
 * @var current_set target current / torque output
 */
typedef struct {
    uint8_t     cur_idx;
    uint16_t    angle[MAXIMUM_STATE];
    int16_t     current_get;
    int16_t     current_set;
}   motor_6623_t;

/**
 * @struct  motor_3510_t
 * @brief   store 3510 motor data
 * @var cur_idx     current index to write into the buffer
 * @var angle       a circular buffer to store current and previous angle
 * @var current_get actual current / torque output
 */
typedef struct {
    uint8_t     cur_idx;
    uint16_t    angle[MAXIMUM_STATE];
    int16_t     current_get;
}   motor_3510_t;

/**
 * @struct  motor_2006_t
 * @brief   store 2006 motor data
 * @var cur_idx     current index to write into the buffer
 * @var angle       a circular buffer to store current and previous angle
 * @var speedRPM    rotational speed in RPM (Rotation Per Minute)
 */
typedef struct {
    uint8_t     cur_idx;
    uint16_t    angle[MAXIMUM_STATE];
    int16_t     speedRPM[MAXIMUM_STATE];
}   motor_2006_t;

/**
 * @union   motor_u
 * @brief   can be interpreted as all kinds of motors
 * @var chassis         chassis motor
 * @var friction        friction pully for shooting
 * @var gimbal          main gimbal
 * @var extra_gimbal    the 3rd axis gimbal
 * @var poke            bullet supply motor
 */
typedef union {
    motor_3508_t chassis;
    motor_3508_t friction;
    motor_6623_t gimbal;
    motor_3510_t extra_gimbal;
    motor_2006_t poke;
}   motor_u;

/**
 * @brief   parse chassis motor data from CAN buffers
 * @param can_id    can bus id choosing from [CAN1, CAN2]
 * @param sensor_id hardware sensor id for a specific motor
 * @param motor     union pointer that stores the parsed results
 * @return 1 if successfully parsed data, otherwise 0
 */
uint8_t get_chassis_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor);

/**
 * @brief   parse friction motor data from CAN buffers
 * @param can_id    can bus id choosing from [CAN1, CAN2]
 * @param sensor_id hardware sensor id for a specific motor
 * @param motor     union pointer that stores the parsed results
 * @return 1 if successfully parsed data, otherwise 0
 */
uint8_t get_friction_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor);

/**
 * @brief   parse main gimbal motor data from CAN buffers
 * @param can_id    can bus id choosing from [CAN1, CAN2]
 * @param sensor_id hardware sensor id for a specific motor
 * @param motor     union pointer that stores the parsed results
 * @return 1 if successfully parsed data, otherwise 0
 */
uint8_t get_gimbal_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor);

/**
 * @brief   parse extra gimbal motor data from CAN buffers
 * @param can_id    can bus id choosing from [CAN1, CAN2]
 * @param sensor_id hardware sensor id for a specific motor
 * @param motor     union pointer that stores the parsed results
 * @return 1 if successfully parsed data, otherwise 0
 */
uint8_t get_extra_gimbal_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor);

/**
 * @brief   parse bullet supply motor data from CAN buffers
 * @param can_id    can bus id choosing from [CAN1, CAN2]
 * @param sensor_id hardware sensor id for a specific motor
 * @param motor     union pointer that stores the parsed results
 * @return 1 if successfully parsed data, otherwise 0
 */
uint8_t get_poke_data(uint8_t can_id, uint8_t sensor_id, motor_u* motor);

/** @} */

#endif
