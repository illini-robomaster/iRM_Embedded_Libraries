/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    tx2.c
 * @brief   TX2 transmit / receive utility
 * @log     2018-04-18 nickelliang
 */

#include "tx2.h"

uint8_t tx2_rx_buffer[2][BSP_TX2_MAX_LEN];  // Double buffer

uint8_t tx2_init(void) {
    /* Initialize REFEREE to IDLE interrupt */
    uart_port_init(&BSP_TX2_PORT);
    /* Enable DMA for RX */
    uart_enable_receiver_dma(&BSP_TX2_PORT);
    /* Enable multibuffer DMA */
    return uart_dma_multibuffer_it(BSP_TX2_PORT.hdmarx, (uint32_t)&BSP_TX2_PORT.Instance->DR, (uint32_t)tx2_rx_buffer[0], (uint32_t)tx2_rx_buffer[1], BSP_TX2_MAX_LEN);
}
