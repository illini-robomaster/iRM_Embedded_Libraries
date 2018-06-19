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
 * @author  Eric_Liang <hanghang177>
 * @date    2018-04-20
 * @file    servo.c
 * @brief   Library Level servo control
 * #log     2018-04-20 hanghang177
 */

#include "servo.h"

void servo_init(servo_t *servo,
                TIM_HandleTypeDef *htim, uint8_t channel_id, uint32_t init_pwm) {
    servo->pwm          = pwm_init(NULL, htim, channel_id);
    servo->init_pwm     = init_pwm;
    servo->current_pwm  = init_pwm;
    servo->max_degree   = STANDARDMAX;
    servo->min_degree   = STANDARDMIN;
    servo->current_angle = pwm_to_degrees(servo, init_pwm);
    pwm_set_pulse_width(servo->pwm, init_pwm);
}

void servo_writeMicroseconds(servo_t *servo,
                uint32_t pwm)  {
    servo->current_pwm      = pwm;
    servo->current_angle    = pwm_to_degrees(servo, pwm);
    pwm_set_pulse_width(servo->pwm, pwm);
}

void servo_write(servo_t *servo,
                uint32_t degrees)  {
    servo->current_angle = degrees;
    servo->current_pwm   = degrees_to_pwm(servo, degrees);
    pwm_set_pulse_width(servo->pwm, servo->current_pwm);
}

void servo_setmax(servo_t *servo,
                 uint32_t degrees)  {
    servo->max_degree = degrees;
}

void servo_setmin(servo_t *servo,
                 uint32_t degrees)  {
    servo->min_degree = degrees;
}

uint32_t pwm_to_degrees(servo_t *servo, uint32_t pwm)  {
    /* Calculation PWM -> Degrees */
    return (pwm - 1000)*(servo->max_degree - servo->min_degree)/1000
           + servo->min_degree;
}

uint32_t degrees_to_pwm(servo_t *servo, uint32_t degrees)  {
    /* Calculation Degrees -> PWM */
    return (degrees - servo->min_degree)*1000
           /(servo->max_degree - servo->min_degree) + 1000;
}
