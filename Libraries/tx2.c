/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    tx2.c
 * @brief   TX2 transmit / receive utility
 * @log     2018-04-18 nickelliang
 */

#include "tx2.h"

uint8_t tx2_init(data_process_t* source) {
    /* Initialize TX2 to IDLE interrupt */
    uart_port_init(source->huart);
    /* Enable DMA for RX */
    uart_enable_rx_dma(source->huart);
    /* Enable multibuffer DMA */
    return uart_dma_multibuffer_it(source->huart->hdmarx, (uint32_t)&source->huart->Instance->DR, (uint32_t)source->buff[0], (uint32_t)source->buff[1], source->buff_size);
}

uint8_t tx2_dispatcher(void *target_struct, data_process_t *process_struct) {
#ifdef DEBUG
    BSP_DEBUG;
    printf("Enter TX2 dispathcer.\r\n");
#endif
    tx2_t *tx2              = target_struct;
    data_process_t *source  = process_struct;
    uint16_t data_length    = source->data_len;
    uint16_t cmdid          = *(uint16_t*)(source->frame_packet + DATA_PROCESS_HEADER_LEN);
    uint8_t *data_addr      = source->frame_packet + DATA_PROCESS_HEADER_LEN + DATA_PROCESS_CMD_LEN;

    switch (cmdid) {
        case CMD_GIMBAL_CONTROL:
            memcpy(&(tx2->gimbal_control), data_addr, data_length);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "Unknown CMDID.");
            return 0;
    }
    return 1;
}

uint8_t tx2_packer(void *target_struct, data_process_t *process_struct, uint16_t cmdid) {
#ifdef DEBUG
    BSP_DEBUG;
    printf("Enter TX2 packer.\r\n");
#endif
    tx2_t *tx2              = target_struct;
    data_process_t *source  = process_struct;
    uint8_t *data_stream    = NULL;
    uint16_t data_length    = 0;

    switch (cmdid) {
        case CMD_AIM_REQUEST:
            data_stream = (uint8_t*)&tx2->gimbal_control;
            data_length = (uint16_t)sizeof(aim_request_t);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "Unknown CMDID.");
            return 0;
    }

    data_to_fifo(cmdid, data_stream, data_length, source); // Put data into tx fifo
    tx2_outgoing(); // Acknowledge data is ready to transmit

    return 1;
}

__weak void tx2_incomming(void) {
    /* Implement this function in task layer */
}

__weak void tx2_outgoing(void) {
    /* Implement this function in task layer */
}

/**
 * Callback function declared in bsp_uart. This is a weak function.
 *
 * @author Nickel_Liang
 * @date   2018-04-18
 */
void uart_tx2_callback(void) {
    tx2_incomming();
}
