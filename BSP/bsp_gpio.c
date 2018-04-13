/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-12
 * @file    bsp_gpio.c
 * @brief   GPIO Board Support Package
 * @log     2018-04-13 nickelliang
 */

#include "bsp_gpio.h"

void gpio_interrupt_init(void) {
    /* For SWITCH_1_Pin */
    HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    /* For SWITCH_2_Pin */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
