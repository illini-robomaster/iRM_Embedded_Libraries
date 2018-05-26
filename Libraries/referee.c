/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    referee.c
 * @brief   Referee system utility
 * @log     2018-05-26 nickelliang
 */

#include "referee.h"

uint8_t referee_init(data_process_t* source) {
    /* Initialize REFEREE to IDLE interrupt */
    uart_port_init(source->huart);
    /* Enable DMA for RX */
    uart_enable_rx_dma(source->huart);
    /* Enable multibuffer DMA */
    return uart_dma_multibuffer_it(source->huart->hdmarx, source->huart->Instance->DR, (uint32_t)(source->buff[0]), (uint32_t)(source->buff[1]), source->buff_size);
}

uint8_t referee_dispatcher(void* target_struct, data_process_t* process_struct) {
    /* @todo Need to consider racing condition here */
#ifdef DEBUG
    BSP_DEBUG;
    print("Enter referee dispatcher.\r\n");
#endif
    referee_t* referee     = target_struct;
    data_process_t* source = process_struct;
    uint16_t data_length   = source->data_len;
    uint16_t cmdid         = *(uint16_t*)(source->frame_packet + DATA_PROCESS_HEADER_LEN);
    uint8_t* data_addr     = source->frame_packet + DATA_PROCESS_HEADER_LEN + DATA_PROCESS_CMD_LEN;

    /* @todo Add frame length check here */
    switch (cmdid) {
        case CMD_GAME_ROBOT_INFO:
            memcpy(&(referee->game_robot_info), data_addr, data_length);
            break;
        case CMD_DAMAGE_DATA:
            memcpy(&(referee->damage_data), data_addr, data_length);
            break;
        case CMD_SHOOT_DATA:
            memcpy(&(referee->shoot_data), data_addr, data_length);
            break;
        case CMD_POWER_HEAT_DATA:
            memcpy(&(referee->power_heat_data), data_addr, data_length);
            break;
        case CMD_RFID_DATA:
            memcpy(&(referee->rfid_data), data_addr, data_length);
            break;
        case CMD_GAME_RESULT:
            memcpy(&(referee->game_result), data_addr, data_length);
            break;
        case CMD_BUFF_DATA:
            memcpy(&(referee->buff_data), data_addr, data_length);
            break;
        case CMD_ROBOT_POSITION:
            memcpy(&(referee->robot_position), data_addr, data_length);
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "Unknown CMDID.");
            return 0;
    }
    return 1;
}

uint8_t referee_packer(void *target_struct, data_process_t *process_struct, uint16_t cmdid) {
#ifdef DEBUG
    BSP_DEBUG;
    printf("Enter referee packer.\r\n");
#endif
    referee_t *referee      = target_struct;
    data_process_t *source  = process_struct;
    uint8_t *data_stream    = NULL;
    uint16_t data_length    = 0;

    switch (cmdid) {
        case CMD_CUSTOM_DATA:
            data_stream = (uint8_t*)&referee->custom_data;
            data_length = (uint16_t)sizeof(custom_data_t);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "Unknown CMDID.");
            return 0;
    }

    data_to_fifo(cmdid, data_stream, data_length, source); // Put data into tx fifo
    referee_outgoing(); // Acknowledge data is ready to transmit

    return 1;
}

__weak void referee_incomming(void) {
    /* Implement this function in task layer */
}

__weak void referee_outgoing(void) {
    /* Implement this function in task layer */
}

/**
 * Callback function declared in bsp_uart. This is a weak function.
 *
 * @author Nickel_Liang
 * @date   2018-04-18
 */
void uart_referee_callback(void) {
    referee_incomming();
}
