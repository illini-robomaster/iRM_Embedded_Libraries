/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-15
 * @file    bsp_key.h
 * @brief   LED board support package
 * @log     2018-04-15 nickelliang
 */

#ifndef _BSP_KEY_
#define _BSP_KEY_

#include "stm32f4xx_hal.h"
#include "main.h"

#define key_pressed !HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) // Read val is 0
#define key_released HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)

#endif
