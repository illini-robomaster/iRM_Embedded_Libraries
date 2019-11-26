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

#ifndef _GIMBAL_H_
#define _GIMBAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pid.h"
#include "motor.h"
#include "dbus.h"
#include "lib_config.h"

/**
 * @ingroup library
 * @defgroup gimbal Gimbal
 * @{
 */

typedef struct {
    float pitch_ang;            // pitch err intergrated based on mouse input
    float yaw_ang;              // yaw error integrated from mouse movement
    pid_ctl_t *pitch;           // pitch motor pid
    int16_t yaw_middle;         // a pre-determined value for yaw motor
    pid_ctl_t *yaw;             // yaw motor pid
    pid_ctl_t *camera_pitch;    // camera pitch motor pid
} gimbal_t;

/**
 * Get feedbacks of motors related to gimbal
 * @brief
 * @param my_gimbal the desired gimbal struct; represents gimbal
 */
void gimbal_update(gimbal_t *my_gimbal);

/**
 * Initialize gimbal motors
 * @brief
 * @param my_gimbal gimbal struct that stores all gimbal related variables
 */
void gimbal_init(gimbal_t *my_gimbal);

/**
 * @brief hang all gimbal motor into an infinite zero-output loop
 * @param my_gimbal gimbal struct that stores all gimbal related variables
 * @return none
 */
void gimbal_kill(gimbal_t *my_gimbal);

/**
 * calculate gimbal movement based on mouse feedback from mouse
 * @brief
 * @param my_gimbal        my gimbal object
 * @param rc               public dbus struct that holds mouse data
 * @param observed_abs_yaw absolute yaw angle with respect to the ground (instead of chassis)
 */
void gimbal_mouse_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw);

/**
 * calculate gimbal movement based on remote controller
 * @brief
 * @param my_gimbal        my gimbal object
 * @param rc               public dbus struct that holds remote controller data
 * @param observed_abs_yaw absolute yaw angle with respect to the ground (instead of chassis)
 */
void gimbal_remote_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw);

/**
 * @brief set a sequence of way point for yaw motor for a large angle smooth and slow turn
 * @param my_gimbal my gimbal object
 * @param delta_ang the amount of angle yaw needs to turn
 * @param step_size angle differences between consecutive waypoints
 */
void yaw_ramp_ctl(gimbal_t *my_gimbal, int32_t delta_ang, uint16_t step_size);

/**
 * @brief set yaw angle (as in gimbal motor relative angles)
 * @param my_gimbal     my gimbal object
 * @param yaw_ang       target yaw angle in motor degree
 */
void gimbal_set_yaw_angle(gimbal_t *my_gimbal, int32_t yaw_ang);

/**
 * Run motor with corresponding values. SHOULD ONLY BE CALLED AFTER PID CALC
 * @brief
 * @param my_gimbal my gimbal object
 */
void run_gimbal(gimbal_t *my_gimbal);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
