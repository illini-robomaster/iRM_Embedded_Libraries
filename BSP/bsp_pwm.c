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
 * @file    bsp_pwm.c
 * @brief   Board support package for pwm servos and escs
 * #log     2018-04-20 hanghang177
 */

#include "bsp_pwm.h"

/*Initialize HTIM5 for PWM*/

void pwm5_init(void) {
    pwm_init(&htim5);
}

/*Transmit PWM signal through HTIM5 Channel 1 - 4*/

void pwm5_transmit(uint32_t channel, uint32_t pulsewidth) {
    switch(channel) {
        case 1:
            pwm_transmit(&htim5, TIM_CHANNEL_1, pulsewidth);
            break;
        case 2:
            pwm_transmit(&htim5, TIM_CHANNEL_2, pulsewidth);
            break;
        case 3:
            pwm_transmit(&htim5, TIM_CHANNEL_3, pulsewidth);
            break;
        case 4:
            pwm_transmit(&htim5, TIM_CHANNEL_4, pulsewidth);
            break;
        default:
            break;
  }
}

/*Initialize General HTIM for PWM*/

static void pwm_init(TIM_HandleTypeDef *htim) {
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4);
}
