/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-16
 * @file    bsp_uart.c
 * @brief   Board support package for uart devices
 * @log     2018-04-17 nickelliang
 */

#include "bsp_uart.h"

uint8_t  dbus_rx_buffer[DBUS_BUF_LEN];

void dbus_init(void) {
    /* Clear the UART IDLE pending flag */
    __HAL_UART_CLEAR_IDLEFLAG(&BSP_DBUS_PORT);
    /* Idle line detection interrupt mode */
    __HAL_UART_ENABLE_IT(&BSP_DBUS_PORT, UART_IT_IDLE);
    /* Start DMA transfer with interrupt disabled */
    uart_rx_dma_without_it(&BSP_DBUS_PORT, dbus_rx_buffer, DBUS_MAX_LEN);
}

uint8_t uart_rx_idle_callback(UART_HandleTypeDef* huart) {
    /* Clear IDEL IT flag, avoid interrupt again, prevent package adhesion */
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    /* Handle received data */
    if (huart == &BSP_DBUS_PORT) {
        /* Only dbus is using no interrupt mode */
        /* Clear DMA transfer complete flag */
        __HAL_DMA_DISABLE(huart->hdmarx);

        /* Handle dbus data from DMA */
        /* @todo Enter critical section here */
        // uint32_t status = taskENTER_CRITICAL_FROM_ISR();
        if ((DBUS_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == DBUS_BUF_LEN) {
            /* @todo Process DBUS data here */
            // rc_callback_handler(&rc, dbus_buf);
            /* @todo Add offline detection for dbus */
            // err_detector_hook(REMOTE_CTRL_OFFLINE);
        }
        /* @todo Exit critical section here */
        // taskEXIT_CRITICAL_FROM_ISR(status);

        /* restart dma transmission */
        __HAL_DMA_SET_COUNTER(huart->hdmarx, DBUS_MAX_LEN);
        __HAL_DMA_ENABLE(huart->hdmarx);
        return 1;
    }
    else if ((huart == &BSP_REFEREE_PORT) || (huart == &BSP_TX2_PORT)) {
        /* @todo Process REFEREE and TX2 data here */
        // uart_idle_interrupt_signal(huart);
        return 1;
    }
    else {
        return 0;
    }
}

static uint8_t uart_rx_dma_without_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t size) {
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
        huart->RxXferSize = size;
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

static uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream) {
    /* Return the number of remaining data units for DMAy Streamx */
    return ((uint16_t)(dma_stream->NDTR));
}
