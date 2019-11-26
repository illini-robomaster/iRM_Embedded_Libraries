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

#ifdef __cplusplus
extern "C" {
#endif

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
            uart_referee_callback();
            /* @todo Add offline detection for referee */
            /* @todo @important Consider flag handling */
            /* reference: */
            // osSignalSet(judge_unpack_task_t, JUDGE_UART_IDLE_SIGNAL);
        }
        else if (huart == &BSP_TX2_PORT) {
            /* @todo Process TX2 data here */
            uart_tx2_callback();
            /* @todo Add offline detection for tx2 */
        }
        else {
            bsp_error_handler(__FUNCTION__, __LINE__, "Undefined active UART device callbacked.");
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif
