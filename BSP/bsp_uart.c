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
 * @date    2018-04-16
 * @file    bsp_uart.c
 * @brief   Board support package for uart devices
 * @log     2018-04-17 nickelliang
 */

#include "bsp_uart.h"

/* ===== UART Utilities ===== */

uint8_t uart_rx_dma_without_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t size) {
    /* Check if we can change UART setting */
    if (huart->RxState == HAL_UART_STATE_READY) {
        /* Check parameter validity */
        if ((pData == NULL) || (size == 0)) {
            bsp_error_handler(__FUNCTION__, __LINE__, "Invalid input.");
            return 0;
        }
        /* Process Locked */
        __HAL_LOCK(huart);
        /* Set UART properties */
        huart->pRxBuffPtr = pData;
        huart->RxXferSize = size;   // NDTR value in fact
        huart->ErrorCode  = HAL_UART_ERROR_NONE;
        /* No need to set Callback functions since we are not using interrupt */
        /* Enable the DMA Stream */
        HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, size);
        /* Enable the DMA transfer for the receiver request by setting the DMAR bit in the UART CR3 register */
        SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
        /* Process Unlocked */
        __HAL_UNLOCK(huart);
        return 1;
    }
    else {
        bsp_error_handler(__FUNCTION__, __LINE__, "UART is busy.");
        return 0;
    }
}

uint8_t uart_dma_multibuffer_it(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength) {
    /* Memory-to-memory transfer not supported in double buffering mode */
    if (hdma->Init.Direction == DMA_MEMORY_TO_MEMORY) {
        hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;
        bsp_error_handler(__FUNCTION__, __LINE__, "Memory to memory transfer not supported.");
        return 0;
    }
    /* Set the UART DMA transfer complete callback */
    /* Current memory buffer used is Memory 1 callback */
    hdma->XferCpltCallback   = dma_m0_rxcplt_callback;
    /* Current memory buffer used is Memory 0 callback */
    hdma->XferM1CpltCallback = dma_m1_rxcplt_callback;
    /* Check callback functions */
    if ((NULL == hdma->XferCpltCallback) || (NULL == hdma->XferM1CpltCallback)) {
        hdma->ErrorCode = HAL_DMA_ERROR_PARAM;
        bsp_error_handler(__FUNCTION__, __LINE__, "Invalid parameter.");
        return 0;
    }
    /* Process locked */
    __HAL_LOCK(hdma);
    if (HAL_DMA_STATE_READY == hdma->State) {
        /* Change DMA peripheral state */
        hdma->State = HAL_DMA_STATE_BUSY;
        /* Initialize the error code */
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;
        /* Enable the Double buffer mode */
        hdma->Instance->CR |= (uint32_t)DMA_SxCR_DBM;
        /* Configure DMA Stream destination address */
        hdma->Instance->M1AR = SecondMemAddress;
        /* Configure DMA Stream data length */
        hdma->Instance->NDTR = DataLength;
        /* Configure the source, destination address */
        if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH) {
            hdma->Instance->PAR = DstAddress;
            hdma->Instance->M0AR = SrcAddress;
        }
        else {
            hdma->Instance->PAR = SrcAddress;
            hdma->Instance->M0AR = DstAddress;
        }
        /* Clear TC flags */
        __HAL_DMA_CLEAR_FLAG (hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));
        /* Enable TC interrupts*/
        hdma->Instance->CR |= DMA_IT_TC;
        /* Enable the peripheral */
        __HAL_DMA_ENABLE(hdma);
        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;
    }
    else {
        bsp_error_handler(__FUNCTION__, __LINE__, "HAL busy.");
        return 0;
    }
    /* Process unlocked */
    __HAL_UNLOCK(hdma);
    return 1;
}

void uart_enable_rx_dma(UART_HandleTypeDef* huart) {
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
}

void uart_port_init(UART_HandleTypeDef* huart) {
    /* Clear the UART IDLE pending flag */
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    /* Idle line detection interrupt mode */
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
}

void uart_tx_blocking(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size) {
    HAL_UART_Transmit(huart, p_data, size, UART_TX_BLOCKING_TIMEOUT);
}

/* ===== DMA Utilities ===== */

uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream) {
    /* Return the number of remaining data units for DMAy Streamx */
    return ((uint16_t)(dma_stream->NDTR));
}

uint8_t dma_current_memory_target(DMA_Stream_TypeDef *dma_stream) {
    /* Return which memory buffer is in use */
    return (uint8_t)((dma_stream->CR & DMA_SxCR_CT) != 0);
}

static void dma_m1_rxcplt_callback(DMA_HandleTypeDef *hdma) {
    /* DMA buffer should never reach upper limit */
}

static void dma_m0_rxcplt_callback(DMA_HandleTypeDef *hdma) {
    /* DMA buffer should never reach upper limit */
}

/* ===== Weak Functions ===== */

__weak void uart_dbus_callback(void) {
    /* Implement this function in dbus library */
}

__weak void uart_referee_callback(void) {
    /* Implement this function in referee library */
}

__weak void uart_tx2_callback(void) {
    /* Implement this function in tx2 library */
}

/**
 * This is a weak function. Indicate end of DMA rx transfer.
 *
 * @param  huart      Which uart to handle
 * @author Nickel_Liang
 * @date   2018-04-19
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    /* @todo Consider add signal handling here */
    if (huart == &BSP_DBUS_PORT) {
        bsp_error_handler(__FUNCTION__, __LINE__, "DBUS RX callbacked.");
    }
    else if (huart == &BSP_REFEREE_PORT) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Referee RX callbacked.");
    }
    else if (huart == &BSP_TX2_PORT) {
        bsp_error_handler(__FUNCTION__, __LINE__, "TX2 RX callbacked.");
    }
    else {
        bsp_error_handler(__FUNCTION__, __LINE__, "Undefined active UART device RX callbacked.");
    }
}

/**
 * This is a weak function. Indicate end of DMA tx transfer.
 *
 * @param  huart      [description]
 * @author Nickel_Liang
 * @date   2018-04-19
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    /* @todo Consider add signal handling here */
    if (huart == &BSP_REFEREE_PORT) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Referee TX callbacked.");
    }
    else if (huart == &BSP_TX2_PORT) {
        bsp_error_handler(__FUNCTION__, __LINE__, "TX2 TX callbacked.");
    }
    else if (huart == &BSP_PRINT_PORT) {

    }
    else {
        bsp_error_handler(__FUNCTION__, __LINE__, "Undefined active UART device TX callbacked.");
    }
}
