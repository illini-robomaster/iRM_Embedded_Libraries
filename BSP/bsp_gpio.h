/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-12
 * @file    bsp_gpio.h
 * @brief   Header of GPIO Board Support Package
 * @log     2018-04-13 nickelliang
 */

#ifndef _BSP_GPIO_
#define _BSP_GPIO_

#include "stm32f4xx_hal.h"
#include "rm_config.h"

/**
 * Init NVIC (Interrupt) of GPIO pins. Used for Trigger Switch read.
 *
 * @author Eric_Liang
 * @date   2018-04-13
 */
void gpio_interrupt_init(void);

#endif
