/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-16
 * @file    bsp_uart.c
 * @brief   Board support package for uart devices
 * @log     2018-04-17 nickelliang
 */

#include "bsp_uart.h"

uint8_t uart_rx_dma_without_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t size) {
    /* Check if we can change UART setting */
    if (huart->RxState == HAL_UART_STATE_READY) {
        /* Check parameter validity */
        if ((pData == NULL) || (size == 0)) {
            bsp_error_handler(__FILE__, __LINE__, "Invalid input.");
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
        bsp_error_handler(__FILE__, __LINE__, "UART is busy.");
        return 0;
    }
}

void uart_port_init(UART_HandleTypeDef* huart) {
    /* Clear the UART IDLE pending flag */
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    /* Idle line detection interrupt mode */
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
}

uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream) {
    /* Return the number of remaining data units for DMAy Streamx */
    return ((uint16_t)(dma_stream->NDTR));
}

__weak void uart_dbus_callback(void) {
    /* Implement this function in dbus library */
}
