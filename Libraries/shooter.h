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

typedef enum {
    CAN,
    PWM,
}   shooter_type_t;

typedef enum {
    FLYWHL_ON,
    FLYWHL_OFF,
}   flywhl_state_t;

typedef struct {
    motor_t     *left;
    motor_t         *right;
    flywhl_state_t  state;
}   flywhl_t;

typedef struct {
    flywhl_t        *flywhl;
    pid_ctl_t       *poker;
    shooter_type_t  type;
}   shooter_t;

static flywhl_t* pwm_flywhl_init(flywhl_t *pwm_flywhl);

static flywhl_t* can_flywhl_init(flywhl_t *can_flywhl);

static pid_ctl_t* poker_init(pid_ctl_t* poker);

shooter_t* shooter_init(shooter_t *shooter, shooter_type_t type);

void flywhl_set_output(shooter_t *shooter, int32_t output);

void flywhl_on(shooter_t *shooter);

void flywhl_off(shooter_t *shooter);

void poker_set_speed(shooter_t *shooter, int32_t speed);

void poker_run(shooter_t *shooter);

#endif
