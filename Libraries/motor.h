/**
 * @author  Alvin Sun
 * @date    2018-04-16
 * @file    motor.h
 * @brief   library level motor data processing
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <inttypes.h>
#include "bsp_can.h"
#include "bsp_error_handler.h"
#include "bsp_print.h"

/**
 * @ingroup library
 * @defgroup motor Motor
 * @{
 */

#define CAN1_ID 1
#define CAN2_ID 2

#define TX1_ID  0x200
#define TX2_ID  0x1FF
#define TX3_ID  0x2FF

#define RX1_MIN 0x201
#define RX1_MAX 0x204
#define RX2_MIN 0x205
#define RX2_MAX 0x208
#define RX3_MIN 0x209
#define RX3_MAX 0x20C

#define MAXIMUM_STATE 4

// CCW and CW are looking towards the aixs
#define ANGLE_MIN_3510      0       // 0    degree
#define ANGLE_MAX_3510      8191    // 360  degree
#define ANGLE_CRT_3510      1       // angle value normal
#define CURRENT_MIN_3510    -29000  // CCW  ~1.3A
#define CURRENT_MAX_3510    29000   // CW   ~1.3A Looking towards LED
#define CURRENT_CRT_3510    1       // current value normal

#define ANGLE_MIN_3508      0       // 0    degree
#define ANGLE_MAX_3508      8191    // 360  degree
#define ANGLE_CRT_3508      -1      // angle value reversed
#define CURRENT_MIN_3508    -16384  // CW   20A
#define CURRENT_MAX_3508    16384   // CCW  20A
#define CURRENT_CRT_3508    1       // current value normal
#define SPEED_CRT_3508      -1      // speed value reversed

#define ANGLE_MIN_6623      0       // 0    degree
#define ANGLE_MAX_6623      8191    // 360  degree
#define ANGLE_CRT_6623      1       // angle value normal
#define CURRENT_MIN_6623    -13000  // CCW  ~5.3A
#define CURRENT_MAX_6623    13000   // CW   ~5.3A
#define CURRENT_CRT_6623    1       // current value normal

#define ANGLE_MIN_2006      0       // 0    degree
#define ANGLE_MAX_2006      8191    // 360  degree
#define ANGLE_CRT_2006      1       // angle value normal
#define CURRENT_MIN_2006    -10000  // CCW  10A
#define CURRENT_MAX_2006    10000   // CW   10A
#define CURRENT_CRT_2006    1       // current value normal
#define SPEED_CRT_2006      1       // speed value noraml

typedef enum {
    M3508,
    M3510,
    M2006,
    M6623
}   motor_type_t;

/**
 * @struct  motor_3508_t
 * @brief   store 3508 motor data
 * @var angle       a circular buffer to store current and previous angle
 * @var current_get actual current / torque output
 * @var speedRPM    rotational speed in RPM (Rotation Per Minute)
 * @var temperature sensor temperature in celcius degree
 */
typedef struct {
    int16_t     angle[MAXIMUM_STATE];
    int16_t     current_get;
    int16_t     speedRPM[MAXIMUM_STATE];
    uint8_t     temperature;
}   motor_3508_t;

/**
 * @struct  motor_6623_t
 * @brief   store 6623 motor data
 * @var angle       a circular buffer to store current and previous angle
 * @var current_get actual current / torque output
 * @var current_set target current / torque output
 */
typedef struct {
    int16_t     angle[MAXIMUM_STATE];
    int16_t     current_get;
    int16_t     current_set;
}   motor_6623_t;

/**
 * @struct  motor_3510_t
 * @brief   store 3510 motor data
 * @var angle       a circular buffer to store current and previous angle
 * @var current_get actual current / torque output
 */
typedef struct {
    int16_t     angle[MAXIMUM_STATE];
    int16_t     current_get;
}   motor_3510_t;

/**
 * @struct  motor_2006_t
 * @brief   store 2006 motor data
 * @var angle       a circular buffer to store current and previous angle
 * @var speedRPM    rotational speed in RPM (Rotation Per Minute)
 */
typedef struct {
    int16_t     angle[MAXIMUM_STATE];
    int16_t     speedRPM[MAXIMUM_STATE];
    int16_t     current_get;
}   motor_2006_t;

/**
 * @union   motor_interp_t
 * @brief   motor data interpretation as all kinds of motors
 * @var m3508           generic 3508 motor
 * @var m3510           generic 3510 motor
 * @var m2006           generic 2006 motor
 * @var m6623           generic 6623 motor
 * @var chassis         chassis motor
 * @var friction        friction pully for shooting
 * @var gimbal          main gimbal
 * @var extra_gimbal    the 3rd axis gimbal
 * @var poke            bullet supply motor
 */
typedef union {
    /* generic motor */
    motor_3508_t m3508;
    motor_3510_t m3510;
    motor_2006_t m2006;
    motor_6623_t m6623;
    /* specific functioning motor */
    motor_3508_t chassis;
    motor_3508_t friction;
    motor_6623_t gimbal;
    motor_3510_t extra_gimbal;
    motor_2006_t poke;
}   motor_interp_t;

/**
 * @struct  motor_t
 * @brief   ultimate structure that holds all information for a motor
 * @var as      a union structure motor interpretation
 * @var cur_idx current index to write into the cicular buffer
 * @var rx_id   hardware sensor id as in CAN address
 * @var can_id  CAN id chosen from [CAN1, CAN2]
 */
typedef struct {
    motor_interp_t  as;
    motor_type_t    type;
    uint8_t         cur_idx;
    uint16_t        rx_id;
    uint16_t        tx_id;
    uint8_t         can_id;
    int16_t         out;
}   motor_t;

/**
 * Print out 3508 data
 *
 * @param  motor      A 3508 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
void print_3508_data(motor_t *motor);

/**
 * Print out 6623 data
 *
 * @param  motor      A 6623 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
void print_6623_data(motor_t *motor);

/**
 * Print out 3510 data
 *
 * @param  motor      A 3510 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
void print_3510_data(motor_t *motor);

/**
 * Print out 2006 data
 *
 * @param  motor      A 2006 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
void print_2006_data(motor_t *motor);

/**
 * @brief initialize generic motor variable with specific sensor id and can id
 * @param motor     motor_t type variable to be initialized
 * @param rx_id     hardware sensor id as in CAN address
 * @param can_id    CAN id chosen from [CAN1_ID, CAN2_ID]
 * @param type      type of the motor
 */
void motor_init(motor_t *motor, 
        uint16_t rx_id, uint8_t can_id, motor_type_t type);

/**
 * @brief get generic motor data (type inferred from the data structure) 
 * @param motor motor_t typed pointer that stores the parsed results
 * @return 1 if successfully parsed data, otherwise 0
 */
uint8_t get_motor_data(motor_t *motor);

/**
 * @brief get the latest angle data from a motor
 * @param motor a motor variable
 * @return the latest angle data
 */
int16_t get_motor_angle(motor_t *motor);

/**
 * @brief get the nth latest angle data from a motor
 * @param motor a motor variable
 * @param n     the nth lastest data to retrieve (e.g. to get the most recent angle data,
 *              set n to 0)
 * @return      the nth latest angle data; 
 */
int16_t get_motor_prev_angle(motor_t *motor, int n);

/**
 * @brief get the first derivative of latest angle data
 * @param motor a motor variable
 * @return the first derivative of latest angle data
 */
int16_t get_motor_d_angle(motor_t *motor);

/**
 * @brief get the second drivative of latest angle data
 * @param motor a motor variable
 * @return the second drivative of latest angle data
 */
int16_t get_motor_dd_angle(motor_t *motor);

/**
 * @brief print out generic motor data (type inferred from the data structure)
 * @param motor motor_t typed pointer that stores the data to be printed
 */
void print_motor_data(motor_t *motor);

/** @} */

#endif
