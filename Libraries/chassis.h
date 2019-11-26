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

#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pid.h"
#include "motor.h"
#include "dbus.h"
#include "referee.h"
#include "lib_config.h"

/**
 * @ingroup library
 * @defgroup chassis Chassis
 * @{
 */

#define Q_PI 0.7853982 // \pi / 4

#define CHASSIS_MOTOR_TYPE M3508

#define FL_MOTOR_CANID 0x201 // front left motor can id
#define FR_MOTOR_CANID 0x202
#define RL_MOTOR_CANID 0x203
#define RR_MOTOR_CANID 0x204

#define EVASIVE_DEADBAND 400
#define YAW_DEADBAND     20 // 22 ~ 1 deg

typedef enum{
    CHASSIS_FL = 0,
    CHASSIS_FR = 1,
    CHASSIS_RL = 2,
    CHASSIS_RR = 3
} chassis_motor_t;

/**
 * Initialize chassis motor pids
 * @brief
 * @param my_chassis ptr to the desired chassis object to be initialized
 */
void chassis_init(pid_ctl_t *my_chassis[4]);

/**
 * calculate chassis move based on keyboard input
 * @brief
 * @param my_chassis An (array) of motors pid that represent chassis
 * @param rc         dbus struct; for getting remote controller data
 * @param yaw_angle  Counterclockwise is positive. In radian. How far is gimbal deviate from chassis
 */
void calc_keyboard_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle, float *prev_vx, float *prev_vy);

/**
 * @brief calculate chassis move based on remote controller
 * @param my_chassis an (array) of motors pid that represent chassis
 * @param rc         dbus struct; for getting remote controller data
 * @param yaw_angle  angle of gimabl deviated from chassis, in radian. (counter colockwise is positive)
 */
void calc_remote_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle, float *prev_vx, float *prev_vy);

/**
 * Makes chassis roate based on yaw motor feedback
 * @brief
 * @param my_chassis my chassis object. An array of pid that represents chassis
 * @param desired_yaw_angle desired relative yaw angle;
 * @param yaw_motor ptr to yaw motor for getting angle feedback.
 */
void adjust_chassis_gimbal_pos(pid_ctl_t *my_chassis[4], int16_t desired_yaw_angle, motor_t *yaw_motor);

//TODO for Roger: finish this god damn doc
/**
 * [evasive_move description]
 * @brief
 * @param [name]           array of motors that control chassis
 * @param cur_yaw_feedback current yaw feedback read from yaw motor
 * @param yaw_motor        ptr to yaw motor
 */
void evasive_move(pid_ctl_t *my_chassis[4], int16_t cur_yaw_feedback, motor_t *yaw_motor);

/**
 * @brief set chassis to forward mode
 */
void chassis_mode_forward(void);

/**
 * @brief set chassis to backward mode
 */
void chassis_mode_backward(void);

/**
 * @brief disable all chassis related movement
 */
void chassis_stop(void);

/**
 * @brief hang all chassis motors to an infinite zero-output loop
 * @param my_chassis[4] array of chassis motor pid controller
 */
void chassis_kill(pid_ctl_t *my_chassis[4]);

/**
 * Run chassis motors. SHOULD ONLY BE CALLED AFTER PID CALC
 * @brief
 * @param my_chassis my chassis object. An array of pid that represents chassis
 */
void run_chassis(pid_ctl_t *my_chassis[4]);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
