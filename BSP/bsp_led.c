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
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-14
 * @file    bsp_led.c
 * @brief   led board support package
 * @log     2018-04-14 nickelliang
 */

#include "bsp_led.h"

void led_red_on(void) {
    led_green_off();
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_RESET);
}

void led_red_off(void) {
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_SET);
}

void led_green_on(void) {
    led_red_off();
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_RESET);
}

void led_green_off(void) {
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_SET);
}

void led_yellow_on(void) {
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_RESET);
}

void led_off(void) {
    led_green_off();
    led_red_off();
}
