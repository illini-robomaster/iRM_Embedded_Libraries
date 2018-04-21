/**
 * @author  Alvin Sun
 * @date    2018-04-16
 * @file    motor.h
 * @brief   library level motor data processing
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

/**
 * @ingroup library
 * @defgroup motor Motor
 * @{
 */

#include <inttypes.h>
#include "bsp_can.h"
#include "bsp_error_handler.h"
#include "bsp_print.h"

#define CAN1_ID 1
#define CAN2_ID 2

#define CAN_TX1_ID  0x200
#define CAN_TX2_ID  0x1FF
#define CAN_TX3_ID  0x2FF

#define CAN_RX1_START 0x201
#define CAN_RX2_START 0x205
#define CAN_RX3_START 0x209
#define CAN_GROUP_SIZE  4

#define MAXIMUM_STATE 4

// CCW and CW are looking towards the aixs
#define ANGLE_MIN_3510      0       // 0    degree
#define ANGLE_MAX_3510      8191    // 360  degree
#define ANGLE_RANGE_3510    8192
#define ANGLE_CRT_3510      1       // angle direction normal
#define CURRENT_MIN_3510    -29000  // CCW  ~1.3A
#define CURRENT_MAX_3510    29000   // CW   ~1.3A Looking towards LED
#define CURRENT_CRT_3510    1      // current direction reversed

#define ANGLE_MIN_3508      0       // 0    degree
#define ANGLE_MAX_3508      8191    // 360  degree
#define ANGLE_RANGE_3508    8192
#define ANGLE_CRT_3508      1       // angle direction normal
#define CURRENT_MIN_3508    -16384  // CW   20A
#define CURRENT_MAX_3508    16384   // CCW  20A
#define CURRENT_CRT_3508    1       // current value normal
#define SPEED_CRT_3508      1       // speed direction noraml

#define ANGLE_MIN_6623      0       // 0    degree
#define ANGLE_MAX_6623      8191    // 360  degree
#define ANGLE_RANGE_6623    8192
#define ANGLE_CRT_6623      1       // angle direction normal
#define CURRENT_MIN_6623    -5000   // CCW  ~5.3A
#define CURRENT_MAX_6623    5000    // CW   ~5.3A
#define CURRENT_CRT_6623    -1      // current direction reversed

#define ANGLE_MIN_2006      0       // 0    degree
#define ANGLE_MAX_2006      8191    // 360  degree
#define ANGLE_RANGE_2006    8192
#define ANGLE_CRT_2006      1       // angle direction normal
#define CURRENT_MIN_2006    -10000  // CW  10A
#define CURRENT_MAX_2006    10000   // CCW   10A
#define CURRENT_CRT_2006    1       // current direction normal
#define SPEED_CRT_2006      1       // speed direction noraml

/**
 * @enum    motor_type_t
 * @brief   a enum type that defines avaiblable motor types
 * @var M3508   3508 motor
 * @var M3510   3510 motor
 * @var M2006   2006 motor
 * @var M6623   6623 motor
 */
typedef enum {
    M3508,
    M3510,
    M2006,
    M6623
}   motor_type_t;

/**
 * @struct  motor_3508_t
 * @brief   store 3508 motor data
 * @var angle       most recent angle data
 * @var current_get actual current / torque output
 * @var speed_rpm   rotational speed in RPM (Rotation Per Minute)
 * @var temperature sensor temperature in celcius degree
 */
typedef struct {
    int16_t     angle;
    int16_t     current_get;
    int16_t     speed_rpm;
    uint8_t     temperature;
}   motor_3508_t;

/**
 * @struct  motor_6623_t
 * @brief   store 6623 motor data
 * @var angle       most recent angle data
 * @var current_get actual current / torque output
 * @var current_set target current / torque output
 */
typedef struct {
    int16_t     angle;
    int16_t     current_get;
    int16_t     current_set;
}   motor_6623_t;

/**
 * @struct  motor_3510_t
 * @brief   store 3510 motor data
 * @var angle       most recent angle data
 * @var current_get actual current / torque output
 */
typedef struct {
    int16_t     angle;
    int16_t     current_get;
}   motor_3510_t;

/**
 * @struct  motor_2006_t
 * @brief   store 2006 motor data
 * @var angle       most recent angle data
 * @var speed_rpm   rotational speed in RPM (Rotation Per Minute)
 */
typedef struct {
    int16_t     angle;
    int16_t     speed_rpm;
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
 * @var out     Motor output to be used; clockwise.
 */
typedef struct {
    motor_interp_t  as;
    motor_type_t    type;
    uint16_t        rx_id;
    uint16_t        tx_id;
    uint8_t         can_id;
    float           out;
}   motor_t;

/**************************************************************************
 *                      Private Function Stars Here                       *
 **************************************************************************/

/**
 * Print out 3508 data
 *
 * @param  motor      A 3508 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
static void print_3508_data(motor_t *motor);

/**
 * Print out 6623 data
 *
 * @param  motor      A 6623 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
static void print_6623_data(motor_t *motor);

/**
 * Print out 3510 data
 *
 * @param  motor      A 3510 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
static void print_3510_data(motor_t *motor);

/**
 * Print out 2006 data
 *
 * @param  motor      A 2006 motor
 * @author Nickel_Liang
 * @date   2018-04-17
 */
static void print_2006_data(motor_t *motor);

/**
 * @brief get 3508 motor from a raw data buffer read from CAN
 * @param motor                 motor data
 * @param buf[CAN_DATA_SIZE]    data buffer
 * @return none
 */
static void get_3508_data(motor_t* motor, uint8_t buf[CAN_DATA_SIZE]);

/**
 * @brief get 3508 motor from a raw data buffer read from CAN
 * @param motor                 motor data
 * @param buf[CAN_DATA_SIZE]    data buffer
 * @return none
 */
static void get_6623_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]);

/**
 * @brief get 6623 motor from a raw data buffer read from CAN
 * @param motor                 motor data
 * @param buf[CAN_DATA_SIZE]    data buffer
 * @return none
 */
static void get_3510_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]);

/**
 * @brief get 3510 motor from a raw data buffer read from CAN
 * @param motor                 motor data
 * @param buf[CAN_DATA_SIZE]    data buffer
 * @return none
 */
static void get_2006_data(motor_t *motor, uint8_t buf[CAN_DATA_SIZE]);

/**
 * @brief get 2006 motor from a raw data buffer read from CAN
 * @param motor                 motor data
 * @param buf[CAN_DATA_SIZE]    data buffer
 * @return none
 */

/**
 * @brief helper function for matching a sequnce of id
 * @param old_id previously matched id (points to a value of 0 if no previous id)
 * @param new_id new id to be compared
 * @return 1 if either id matches or previous id does not exist, otherwise 0
 */
static uint8_t match_id(uint16_t *old_id, uint16_t new_id);

/**
 * @brief helper function for clipping value into [-range, range]
 * @param val   value to be clipped
 * @param range maximum absolute range
 * @return the clipped value
 */
static int16_t clip(int16_t val, int16_t range);

/**
 * @brief limit current output to prevent from buring the motor
 * @param val   current output value
 * @param low   lower limit
 * @param high  upper limit
 * @return clipped current output 
 */
static int16_t current_limit(float val, int16_t low, int16_t high);

/**
 * @brief correct motor output direction given the specification of a motor type
 * @note the corrected positive direction is defined to be CCW
 * @param motor motor variable which contains the output to be corrected
 * @return the corrected motor output value
 */
static int16_t correct_output(motor_t *motor);


/**************************************************************************
 *                       Public Function Stars Here                       *
 **************************************************************************/

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
 * @brief set output of a group of 4 motors
 *       function work properly
 * @param motor1 motor #1
 * @param motor2 motor #2
 * @param motor3 motor #3
 * @param motor4 motor #4
 * @note you need to set motor1.out, motor2.out, etc. to make this
 * @note four motors are positioned strictly in order.
 *       Put NULL to any position where you don't want to send any information.
 * @return 1 if successfully sent, 0 for can / tx id inconsistency among
 *         4 motors
 */
uint8_t set_motor_output(motor_t *motor1, motor_t *motor2,
        motor_t *motor3, motor_t *motor4);

/**
 * @brief get the latest angle data from a motor
 * @param motor a motor variable
 * @return the latest angle data
 */
int16_t get_motor_angle(motor_t *motor);

/**
 * @brief calculate angle error given a target angle
 * @param motor     a motor variable
 * @param target    target angle
 * @return error angle given by target angle - current angle
 */
int16_t get_angle_err(motor_t *motor, int16_t target);

/**
 * @brief clip the error to stay in bound
 * @param motor     a motor variable
 * @param err       error value to be clipped
 * @return clipped error value
 */
int16_t clip_angle_err(motor_t *motor, int16_t err);

/**
 * @brief calculate rotation speed error given a target speed
 * @param motor     a motor variable
 * @param target    target speed
 * @return error speed given by target speed - current speed
 */
int16_t get_speed_err(motor_t *motor, int16_t target);

/**
 * @brief print out generic motor data (type inferred from the data structure)
 * @param motor motor_t typed pointer that stores the data to be printed
 */
void print_motor_data(motor_t *motor);

/** @} */

#endif
