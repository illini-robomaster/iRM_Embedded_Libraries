/**
 * @author  Nickel_Liang
 * @date    2018-04-12
 * @file    rm_config.h
 * @brief   RM General Constant Configuration
 * @log     2018-04-13 Nickel_Liang
 * @note    Macros should be injected via cmake
 * @note    All robot dependent constants and parameters should be define in this file.
 * @note    This file should be included in all other .h files.
 */

#ifndef _RM_CONFIG_
#define _RM_CONFIG_

/* Inject macros here if you do not know how to... */

/* Constants and Parameters for Debug Configurations */
/* @note    Macros should be injected via cmake */
#ifdef  DEBUG

#endif

/* Constants and Parameters for all infantries */
/* @note    Macros should be injected via cmake */
// #ifdef (INFANTRY1 | INFANTRY2 | INFANTRY3)
/* CAN BUS ID Definition */
#define CHASSIS_TX_ID               1
#define CHASSIS_CAN_BUS             1
#define CHASSIS_MOTOR               M3508
#define CHASSIS_FRONT_LEFT_ID       1
#define CHASSIS_FRONT_RIGHT_ID      2
#define CHASSIS_REAR_LEFT_ID        3
#define CHASSIS_REAR_RIGHT_ID       4

#define THIRD_AXIS_TX_ID            2
#define THIRD_AXIS_CAN_BUS          1
#define THIRD_AXIS_MOTOR            GM3510
#define THIRD_AXIS_RX_ID            5

#define FRICTION_TX_ID              2
#define FRICTION_CAN_BUS            1
#define FRICTION_MOTOR              M3508
#define FRICTION_LEFT_ID            6
#define FRICTION_RIGHT_ID           7

#define POKE_TX_ID                  2
#define POKE_CAN_BUS                1
#define POKE_1_MOTOR                M3508
#define POKE_1_ID                   8

#define GIMBAL_TX_ID                3
#define GIMBAL_CAN_BUS              1
#define GIMBAL_YAW_MOTOR            6623
#define GIMBAL_YAW_ID               9
#define GIMBAL_PITCH_MOTOR          6623
#define GIMBAL_PITCH_ID             10
// #endif

/* Constants and Parameters for Infantry Robot 1 */
/* @note    Macros should be injected via cmake */
#ifdef  INFANTRY_1

#endif

/* Constants and Parameters for Infantry Robot 2 */
/* @note    Macros should be injected via cmake */
#ifdef  INFANTRY_2
#endif

/* Constants and Parameters for Infantry Robot 3 */
/* @note    Macros should be injected via cmake */
#ifdef  INFANTRY_3
#endif

/* Constants and Parameters for Hero Robot */
/* @note    Macros should be injected via cmake */
#ifdef  HERO
#endif

/* Constants and Parameters for Engineering Robot */
/* @note    Macros should be injected via cmake */
#ifdef  ENGINEERING
/* CAN BUS ID Definition */
#define CHASSIS_TX_ID               1
#define CHASSIS_CAN_BUS             1
#define CHASSIS_MOTOR               M3508
#define CHASSIS_FRONT_LEFT_ID       1
#define CHASSIS_FRONT_RIGHT_ID      2
#define CHASSIS_REAR_LEFT_ID        3
#define CHASSIS_REAR_RIGHT_ID       4

#define SUCTION_TX_ID               2
#define SUCTION_CAN_BUS             1
#define SUCTION_MOTOR               M2006
#define SUCTION_RX_ID               5

#define GIMBAL_TX_ID                3
#define GIMBAL_CAN_BUS              1
#define GIMBAL_YAW_MOTOR            6623
#define GIMBAL_YAW_ID               9
#define GIMBAL_PITCH_MOTOR          GM3510
#define GIMBAL_PITCH_ID             10
#endif

/* Constants and Parameters for Supply Station */
/* @note    Macros should be injected via cmake */
#ifdef  SUPPLY
#endif

/* Constants and Parameters for Sentry Robot */
/* @note    Macros should be injected via cmake */
#ifdef  SENTRY
#endif

/* General Parameters */
/**
 * @todo    Add support for more flexible CAN setting
 */
#define REFEREE_PORT                6
#define TX2_PORT                    3
#define PRINTF_PORT                 6

/* General Constants */
#define PI                          3.14159265358979f

#endif
