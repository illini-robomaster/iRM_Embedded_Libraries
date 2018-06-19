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

#include "bsp_pwm.h"
#include "FreeRTOS.h"
#include "bsp_error_handler.h"

pwm_t *pwm_init(pwm_t *my_pwm, TIM_HandleTypeDef *htim, uint8_t channel) {
    if (!my_pwm)
        my_pwm = pvPortMalloc(sizeof(pwm_t));
    
    my_pwm->htim = htim;
    switch(channel) {
        case 1:
            my_pwm->channel = TIM_CHANNEL_1;
            break;
        case 2:
            my_pwm->channel = TIM_CHANNEL_2;
            break;
        case 3:
            my_pwm->channel = TIM_CHANNEL_3;
            break;
        case 4:
            my_pwm->channel = TIM_CHANNEL_4;
            break;
        deault:
            bsp_error_handler(__FUNCTION__, __LINE__, "pwm channel not valid");
            return NULL;
    }

    HAL_TIM_PWM_Start(my_pwm->htim, my_pwm->channel);
    return my_pwm;
}

void pwm_start(pwm_t *my_pwm) {
    HAL_TIM_PWM_Start(my_pwm->htim, my_pwm->channel);
}

void pwm_stop(pwm_t *my_pwm) {
    HAL_TIM_PWM_Stop(my_pwm->htim, my_pwm->channel);
}

void pwm_set_pulse_width(pwm_t *my_pwm, uint32_t pulse_width) {
    __HAL_TIM_SET_COMPARE(my_pwm->htim, my_pwm->channel, pulse_width);
}
