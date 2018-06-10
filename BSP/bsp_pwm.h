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
 * @file    bsp_pwm.h
 * @brief   Board support package for pwm servos and escs
 * #log     2018-04-20 hanghang177
 */

#ifndef _BSP_PWM_H_
#define _BSP_PWM_H_

#include "tim.h"

/**
 * @ingroup bsp
 * @defgroup bsp_pwm BSP PWM
 * @{
 */

/**
 * A simpler name for writing PWM
 */

#define pwm_transmit __HAL_TIM_SET_COMPARE

/**
 * PWM5 init wrapper
 */

void pwm5_init(void);

/**
 * PWM5 trasmit data
 * @param  channel     Channel number in PWM5
 * @param  pulsewidth  PWM pulsewidth (keep within 1000-2000)
 */

void pwm5_transmit(uint32_t channel, uint32_t pulsewidth);

/**
 * PWM init wrapper
 * @param htim HTIM ID to use (Default: htim5)
 */

static void pwm_init(TIM_HandleTypeDef *htim);

/** @} */

#endif
