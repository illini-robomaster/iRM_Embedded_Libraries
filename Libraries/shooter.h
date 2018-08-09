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

#ifndef _SHOOTER_H_
#define _SHOOTER_H_

#include "pid.h"
#include "motor.h"
#include "FreeRTOS.h"

/**
 * @ingroup library
 * @defgroup shooter Shooter
 * @{
 */

/**
 * @enum flywhl_type_t
 * @brief defines flywheel control type
 */
typedef enum {
    CAN,
    PWM,
}   flywhl_type_t;

/**
 * @enum flywhl_state_t
 * @brief defines flywheel states
 */
typedef enum {
    FLYWHL_ON,
    FLYWHL_OFF,
}   flywhl_state_t;

/**
 * @struct flywhl_t
 * @brief flywheel structure
 * @var left    pointer to the left flywheel motor
 * @var right   pointer to the right flywheel motor
 * @var state   state of the flywheel chosen from [FLYWHL_ON, FLYWHL_OFF]
 */
typedef struct {
    motor_t         *left;
    motor_t         *right;
    flywhl_state_t  state;
}   flywhl_t;

/**
 * @struct shooter_t
 * @brief shooter structure
 * @var flywhl  flywheel
 * @var poker   a pid controller that controls poker motor
 * @var type    flywheel type chosen from [CAN, PWM]
 */
typedef struct {
    flywhl_t        *flywhl;
    pid_ctl_t       *poker;
    flywhl_type_t  type;
}   shooter_t;

/**
 * @brief initialize and return a PWM type flywheel data struct
 * @param pwm_flywhl flywheel data to be initialized. pass in NULL will
 *                   result in dynamically allocating a new flywhl_t instance
 * @return initialized PWM flywheel data struct
 */
static flywhl_t* pwm_flywhl_init(flywhl_t *pwm_flywhl);

/**
 * @brief initialize and return a CAN type flywheel data struct
 * @param can_flywhl flywheel data to be initialized. pass in NULL will
 *                   result in dynamically allocating a new flywhl_t instance
 * @return initialized CAN flywheel data struct
 */
static flywhl_t* can_flywhl_init(flywhl_t *can_flywhl);

/**
 * @brief initialize a pid controller for a bullet supply (what we call it a poker) motor
 * @param poker poker pid controller to be initialized. pass in NULL will
 *              result in dynamically allocating a new pid_ctl_t instance
 * @return initialized pid controller for a poker motor
 */
static pid_ctl_t* poker_init(pid_ctl_t* poker);

/**
 * @brief initialize a shooter
 * @param shooter   shooter to be initialized. passing in NULL will
 *                  result in allocating a new shooter_t instance
 * @param type      flywheel type chosen from [CAN, PWM]
 * @return  initialized shooter 
 */
shooter_t* shooter_init(shooter_t *shooter, flywhl_type_t type);

/**
 * @brief set flywheel output of a shooter
 * @param shooter the shooter
 * @param output output value
 */
void flywhl_set_output(shooter_t *shooter, int32_t output);

/**
 * @brief turn on the flywheel with a default speed defined in lib_config.h
 * @param shooter the shooter
 */
void flywhl_on(shooter_t *shooter);

/**
 * @brief turn off the flywheel of a shooter
 * @param shooter the shooter
 */
void flywhl_off(shooter_t *shooter);

/**
 * @brief set the target speed of a bullet supply motor of a shooter
 * @param shooter   the shooter
 * @param speed     target speed
 */
void poker_set_speed(shooter_t *shooter, int32_t speed);

/**
 * @brief send control command to run both flywheel and poker motors in action
 * @param shooter the shooter
 */
void poker_run(shooter_t *shooter);

/** @} */

#endif
