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
        /* Clear IDEL IT flag, avoid interrupt again, prevent package adhesion */
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        /* Handle received data */
        if (huart == &BSP_DBUS_PORT) {
            /* @todo Do some experiment to adapt same method as referee and tx2 */
            /* Only dbus is using no interrupt mode */
            /* Clear DMA transfer complete flag */
            __HAL_DMA_DISABLE(huart->hdmarx);
            /* Process DBUS data */
            /* @todo Consider use flag to handle dbus data here */
            uart_dbus_callback();
            /* restart dma transmission */
            __HAL_DMA_SET_COUNTER(huart->hdmarx, BSP_DBUS_MAX_LEN);
            __HAL_DMA_ENABLE(huart->hdmarx);
        }
        else if (huart == &BSP_REFEREE_PORT) {
            /* @todo Process REFEREE data here */

            /* @todo Add offline detection for referee */
            /* @todo @important Consider flag handling */
            /* reference: */
            // osSignalSet(judge_unpack_task_t, JUDGE_UART_IDLE_SIGNAL);
        }
        else if (huart == &BSP_TX2_PORT) {
            /* @todo Process TX2 data here */
            /* @todo Add offline detection for tx2 */
        }
        else {
            bsp_error_handler(__FILE__, __LINE__, "Undefined active UART device callbacked.");
        }
    }
}

#endif
