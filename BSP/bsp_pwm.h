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

#ifndef _BSP_PWM_H_
#define _BSP_PWM_H_

#include "tim.h"

/**
 * @ingroup bsp
 * @defgroup bsp_pwm BSP PWM
 * @{
 */

typedef struct {
    TIM_HandleTypeDef   *htim;
    uint32_t            channel;
}   pwm_t;

/**
 * @brief initialize a pwm channel and start sending pwm waveform
 * @param my_pwm    a pwm structure to be initialized
 * @param htim      hal timer handle type
 * @param channel   pwm channel (should be in the range [1 4])
 * @return  initialized pwm structure
 */
pwm_t *pwm_init(pwm_t *my_pwm, TIM_HandleTypeDef *htim, uint8_t channel);

/**
 * @brief start sending pwm waves
 * @param my_pwm    pwm struct variable
 */
void pwm_start(pwm_t *my_pwm);

/**
 * @brief stop sending pwm waves
 * @param my_pwm    pwm struct variable
 */
void pwm_stop(pwm_t *my_pwm);

/**
 * @brief adjust pulse width for a pwm channel
 * @param my_pwm        pwm instance to be adjusted
 * @param pulse_width   pulse width as in timer clock cycles
 * @note pulse_width can mostly mean microseconds if the prescaler are set to enforce a 1MHz timer clock frequency
 */
void pwm_set_pulse_width(pwm_t *my_pwm, uint32_t pulse_width);

/** @} */

#endif
