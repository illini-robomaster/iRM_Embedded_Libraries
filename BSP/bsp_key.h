/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-15
 * @file    bsp_key.h
 * @brief   LED board support package
 * @log     2018-04-15 nickelliang
 */

#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "stm32f4xx_hal.h"
#include "main.h"

/**
 * @ingroup bsp
 * @defgroup bsp_key BSP Key
 * @{
 */

/* This is a naive implementation of key press */
/* @todo Should change key press to interrupt mode */
#define key_pressed !HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) // Read val is 0
#define key_released HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)

/** @} */

#endif
