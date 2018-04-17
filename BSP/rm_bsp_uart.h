/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-17
 * @file    rm_bsp_uart.h
 * @brief   RM bsp for uart insertion file
 * @log     2018-04-17 nickelliang
 * @note    This file is included in ../Src/stm32f4xx_it.c to minimize the impact of future driver and middleware update.
 * @note    All functions should begin with "RM_" due to the nature of interaction with driver codes.
 */

/** @defgroup library User Library */
/** @defgroup bsp BSP Library */

#ifndef _RM_BSP_UART_H_
#define _RM_BSP_UART_H_

#include "bsp_uart.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

/**
 * Interrupt callback handler for IDLE interrupt
 *
 * @author  Nickel Liang
 * @date    2018-04-17
 * @todo    Try to use original callback function
 */
extern inline void RM_UART_IRQHandler(UART_HandleTypeDef *huart) {
    /* Check if interrupt comming from IDLE interrupt */
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE)) {
        /* Process callback conditions in bsp layer */
        uart_rx_idle_callback(huart);
    }
}

#endif
