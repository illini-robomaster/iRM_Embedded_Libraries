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
#include <stdlib.h>
#include "bsp_can.h"
#include "bsp_pwm.h"
#include "bsp_error_handler.h"
#include "bsp_print.h"

#define CAN1_ID 1
#define CAN2_ID 2

#define CAN_TX1_ID          0x200
#define CAN_TX2_ID          0x1FF
#define CAN_TX3_ID          0x2FF

#define CAN_RX1_START 0x201
#define CAN_RX2_START 0x205
#define CAN_RX3_START 0x209
#define CAN_GROUP_SIZE  4

#define MAXIMUM_STATE 4

// CCW and CW are looking towards the aixs
#define ANGLE_MIN_3510      0       // 0    degree
#define ANGLE_MAX_3510      8191    // 360  degree
#define ANGLE_CRT_3510      1       // angle direction normal
#define CURRENT_MIN_3510    -29000  // CCW  ~1.3A
#define CURRENT_MAX_3510    29000   // CW   ~1.3A Looking towards LED
#define CURRENT_CRT_3510    1       // current direction normal

#define ANGLE_MIN_3508      0       // 0    degree
#define ANGLE_MAX_3508      8191    // 360  degree
#define ANGLE_CRT_3508      1       // angle direction normal
#define CURRENT_MIN_3508    -12288  // CW   20A
#define CURRENT_MAX_3508    12288   // CCW  20A
#define CURRENT_CRT_3508    1       // current value normal
#define SPEED_CRT_3508      1       // speed direction noraml

#define ANGLE_MIN_6623      0       // 0    degree
#define ANGLE_MAX_6623      8191    // 360  degree
#define ANGLE_CRT_6623      1       // angle direction normal
#define CURRENT_MIN_6623    -5000   // CCW  ~5.3A
#define CURRENT_MAX_6623    5000    // CW   ~5.3A
#define CURRENT_CRT_6623    -1      // current direction reversed

#define ANGLE_MIN_2006      0       // 0    degree
#define ANGLE_MAX_2006      8191    // 360  degree
#define ANGLE_CRT_2006      1       // angle direction normal
#define CURRENT_MIN_2006    -10000  // CW  10A
#define CURRENT_MAX_2006    10000   // CCW   10A
#define CURRENT_CRT_2006    1       // current direction normal
#define SPEED_CRT_2006      1       // speed direction noraml

#define CURRENT_MIN_2305    0
#define CURRENT_MAX_2305    400

#define ANGLE_RANGE_DJI     8192

#define MOTOR_2_RAD  0.0007669904f  // (360 / 8192) * (pi / 180)
#define DEG_2_MOTOR  22.75556f      // (8192 / 360)

/**
 * @enum    motor_type_t
 * @brief   a enum type that defines avaiblable motor types
 * @var M3508   3508 motor
 * @var M3510   3510 motor
 * @var M2006   2006 motor
 * @var M6623   6623 motor
 */
typedef enum {
    /* can motors */
    M3508,
    M3510,
    M2006,
    M6623,
    MDJICAN,
    /* pwm motors */
    M2305,
    MPWM,
}   motor_type_t;

/**
 * @struct  motor_3508_t
 * @brief   store 3508 motor data
 * @var rx_id   sensor CAN recieve id
 * @var tx_id   sensor CAN transmit id
 * @var can_id  CAN id chosen from [CAN1, CAN2]
 * @var angle       most recent angle data
 * @var current_get actual current / torque output
 * @var speed_rpm   rotational speed in RPM (Rotation Per Minute)
 * @var temperature sensor temperature in celcius degree
 */
typedef struct {
    uint16_t    rx_id;
    uint16_t    tx_id;
    uint8_t     can_id;
    int16_t     angle;
    int16_t     current_get;
    int16_t     speed_rpm;
    uint8_t     temperature;
}   motor_3508_t;

/**
 * @struct  motor_6623_t
 * @brief   store 6623 motor data
 * @var rx_id   sensor CAN recieve id
 * @var tx_id   sensor CAN transmit id
 * @var can_id  CAN id chosen from [CAN1, CAN2]
 * @var angle       most recent angle data
 * @var current_get actual current / torque output
 * @var current_set target current / torque output
 */
typedef struct {    
    uint16_t    rx_id;
    uint16_t    tx_id;
    uint8_t     can_id;
    int16_t     angle;
    int16_t     current_get;
    int16_t     current_set;
}   motor_6623_t;

/**
 * @struct  motor_3510_t
 * @brief   store 3510 motor data
 * @var rx_id   sensor CAN recieve id
 * @var tx_id   sensor CAN transmit id
 * @var can_id  CAN id chosen from [CAN1, CAN2]
 * @var angle       most recent angle data
 * @var current_get actual current / torque output
 */
typedef struct {
    uint16_t    rx_id;
    uint16_t    tx_id;
    uint8_t     can_id;
    int16_t     angle;
    int16_t     current_get;
}   motor_3510_t;

/**
 * @struct  motor_2006_t
 * @brief   store 2006 motor data
 * @var rx_id   sensor CAN recieve id
 * @var tx_id   sensor CAN transmit id
 * @var can_id  CAN id chosen from [CAN1, CAN2]
 * @var angle       most recent angle data
 * @var speed_rpm   rotational speed in RPM (Rotation Per Minute)
 */
typedef struct {
    uint16_t    rx_id;
    uint16_t    tx_id;
    uint8_t     can_id;
    int16_t     angle;
    int16_t     speed_rpm;
    int16_t     current_get;
}   motor_2006_t;

/**
 * @struct  motor_dji_t
 * @brief   store generic dji motor data
 * @var rx_id   sensor CAN recieve id
 * @var tx_id   sensor CAN transmit id
 * @var can_id  CAN id chosen from [CAN1, CAN2]
 * @var angle   most recent angle data
 */
typedef struct {
    uint16_t    rx_id;
    uint16_t    tx_id;
    uint8_t     can_id;
    int16_t     angle;
}   motor_dji_can_t;

typedef struct {
    pwm_t       *pwm;
    uint32_t    idle_throttle;
}   motor_pwm_t;

/**
 * @union   motor_interp_t
 * @brief   motor data interpretation as all kinds of motors
 * @var m3508           generic 3508 motor
 * @var m3510           generic 3510 motor
 * @var m2006           generic 2006 motor
 * @var m6623           generic 6623 motor
 * @var mdjican         generic dji can motor
 * @var mpwm            generic pwm motor
 * @var chassis         chassis motor
 * @var friction        friction pully for shooting
 * @var gimbal          main gimbal
 * @var extra_gimbal    the 3rd axis gimbal
 * @var poke            bullet supply motor
 */
typedef union {
    /* generic motor */
    motor_3508_t        m3508;
    motor_3510_t        m3510;
    motor_2006_t        m2006;
    motor_6623_t        m6623;
    motor_dji_can_t     mdjican;
    motor_pwm_t         mpwm;
    /* specific functioning motor */
    motor_3508_t        chassis;
    motor_3508_t        friction;
    motor_6623_t        gimbal;
    motor_3510_t        extra_gimbal;
    motor_2006_t        poke;
}   motor_interp_t;

/**
 * @struct  motor_t
 * @brief   ultimate structure that holds all information for a motor
 * @var as      a union structure motor interpretation
 * @var cur_idx current index to write into the cicular buffer
 * @var out     Motor output to be used; clockwise.
 */
typedef struct {
    motor_interp_t  as;
    motor_type_t    type;
    float           target;
    float           out;
}   motor_t;

/**************************************************************************
 *                      Private Function Starts Here                      *
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
int16_t clip(int16_t val, int16_t range);

/**
 * @brief limit current output to prevent from buring the motor
 * @param val   current output value
 * @param lim   absolute current output limit
 * @return clipped current output
 */
static int16_t current_limit(float val, int16_t lim);

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
 * @brief initialize generic can motors with specific sensor id and can id
 * @param motor     motor_t type variable to be initialized
 * @param rx_id     hardware sensor id as in CAN address
 * @param can_id    CAN id chosen from [CAN1_ID, CAN2_ID]
 * @param type      type of the motor
 * @return initialized motor pointer
 */
motor_t *can_motor_init(motor_t *motor,
        uint16_t rx_id, uint8_t can_id, motor_type_t type);

/**
 * @brief initialize generic pwm motors with specific pulse width range
 * @param motor     motor_t type variable to be initialized
 * @param pwm       pwm controller
 * @param min_pulse minimum pulse width as in timer clock cycle
 * @param max_pulse maximum pulse width as in timer clock cycle
 * @return initialized motor pointer
 */
motor_t *pwm_motor_init(motor_t *motor, motor_type_t type,
        pwm_t *pwm, uint32_t idle_throttle);

/**
 * @brief get generic motor data (type inferred from the data structure)
 * @param motor motor_t typed pointer that stores the parsed results
 * @return 1 if successfully parsed data, otherwise 0
 */
uint8_t get_motor_data(motor_t *motor);

/**
 * @brief set output of a group of 4 can protocol motors
 * @param motor1 motor #1
 * @param motor2 motor #2
 * @param motor3 motor #3
 * @param motor4 motor #4
 * @note you need to set motor1.out, motor2.out, etc. to make this function work properly
 * @note four motors are positioned strictly in order.
 *       Put NULL to any position where you want to send 0 output.
 * @return 1 if successfully sent, 0 for can / tx id inconsistency among
 *         4 motors
 */
uint8_t set_can_motor_output(motor_t *motor1, motor_t *motor2,
        motor_t *motor3, motor_t *motor4);

/**
 * @brief set output of a single pwm motor
 * @param motor motor instance
 * @return none
 */
void set_pwm_motor_output(motor_t *motor);

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
