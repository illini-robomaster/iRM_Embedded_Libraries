/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    referee.c
 * @brief   Referee system utility
 * @log     2018-04-18 nickelliang
 */

#include "referee.h"

uint8_t referee_rx_buffer[2][BSP_REFEREE_MAX_LEN];  // Double buffer

uint8_t referee_init(void) {
    /* Initialize REFEREE to IDLE interrupt */
    uart_port_init(&BSP_REFEREE_PORT);
    /* Enable DMA for RX */
    uart_enable_rx_dma(&BSP_REFEREE_PORT);
    /* Enable multibuffer DMA */
    return uart_dma_multibuffer_it(BSP_REFEREE_PORT.hdmarx, (uint32_t)&BSP_REFEREE_PORT.Instance->DR, (uint32_t)referee_rx_buffer[0], (uint32_t)referee_rx_buffer[1], BSP_REFEREE_MAX_LEN);
}

/**
 * Callback function declared in bsp_uart. This is a weak function.
 *
 * @author Nickel_Liang
 * @date   2018-04-18
 */
void uart_referee_callback(void) {
    return;
}
