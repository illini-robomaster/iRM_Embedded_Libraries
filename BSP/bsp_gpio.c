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
 * @date    2018-04-12
 * @file    bsp_gpio.c
 * @brief   GPIO Board Support Package
 * @log     2018-04-13 nickelliang
 */

#include "bsp_gpio.h"
#include "FreeRTOS.h"

gpio_t *gpio_init(gpio_t *gpio, GPIO_TypeDef *group, uint16_t pin) {
    if (!gpio)
        gpio = pvPortMalloc(sizeof(gpio_t));

    gpio->group = group;
    gpio->pin   = pin;
    gpio->state = LOW;

    return gpio;
}

void gpio_low(gpio_t *gpio) {
    HAL_GPIO_WritePin(gpio->group, gpio->pin, GPIO_PIN_RESET);
    gpio->state = LOW;
}

void gpio_high(gpio_t *gpio) {
    HAL_GPIO_WritePin(gpio->group, gpio->pin, GPIO_PIN_SET);
    gpio->state = HIGH;
}

gpio_state_t gpio_read(gpio_t *gpio) {
    gpio->state = (gpio_state_t)HAL_GPIO_ReadPin(gpio->group, gpio->pin);
    return gpio->state;
}

/* @todo Need to add support for upto 5 switches */
/**
 * This is a weak function, declared in stm32f4xx_hal_gpio.c
 *
 * @param  GPIO_Pin   A GPIO pin that can generate external interrupt signal
 * @author Nickel_Liang
 * @date   2018-05-27
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    gpio_interrupt(GPIO_Pin);
}

__weak void gpio_interrupt(uint16_t gpio_pin) {
    UNUSED(gpio_pin);
}
