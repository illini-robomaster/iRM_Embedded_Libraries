/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-19
 * @file    data_process.c
 * @brief   Process uart received data in DMA buffer
 * @log     2018-04-19 nickelliang
 */

#include "data_process.h"

data_process_t* data_process_init(UART_HandleTypeDef* huart, osMutexId mutex, uint32_t fifo_size, uint16_t buffer_size, uint8_t sof, uint8_t(*dispatcher)(void *, uint8_t *), void* source_struct) {
    if ((huart == NULL) || (mutex == NULL) || (buffer_size == 0) || (fifo_size == 0) || (dispatcher == NULL) || (source_struct == NULL)) {
        bsp_error_handler(__FILE__, __LINE__, "Invalid parameter.");
        return NULL;
    }

    data_process_t* source  = (data_process_t*)malloc(sizeof(data_process_t));   /* Initialize a data process instance */
    if (source == NULL) {
        bsp_error_handler(__FILE__, __LINE__, "Unable to allocate data process object.");
        return NULL;
    }

    source->huart           = huart;
    source->buff_size       = buffer_size;
    source->read_index      = 0;
    source->source_struct   = source_struct;
    source->dispatcher      = dispatcher;
    source->sof             = sof;
    source->data_len        = 0;
    source->frame_index     = 0;

    source->data_fifo       = fifo_s_create(fifo_size, mutex);
    if (source->data_fifo == NULL) {
        bsp_error_handler(__FILE__, __LINE__, "Unable to allocate FIFO for data process object.");
        free(source);
        return NULL;
    }

    source->buff[0]         = (uint8_t*)malloc(2 * source->buff_size);
    if (source->buff[0] == NULL) {
        bsp_error_handler(__FILE__, __LINE__, "Unable to allocate DMA buffer for data process object.");
        fifo_s_destory(source->data_fifo);
        free(source);
        return NULL;
    }
    source->buff[1]         = source->buff[0] + source->buff_size;

    source->frame_packet    = (uint8_t*)malloc(fifo_size);
    if (source->frame_packet == NULL) {
        bsp_error_handler(__FILE__, __LINE__, "Unable to allocate frame packet buffer for data process object.");
        free(source->buff[0]);
        fifo_s_destory(source->data_fifo);
        free(source);
        return NULL;
    }

    return source;
}

uint8_t buffer_to_fifo(data_process_t* source) {
    if (source == NULL) {
        bsp_error_handler(__FILE__, __LINE__, "Invalid parameter.");
        return 0;
    }

    int32_t write_len;                  /* Helper to calculate how many bytes to write */
    uint8_t* buff = source->buff[0];    /* Source data buffer starting addr */
    uint16_t write_index;               /* Helper to store where our new head is */

#ifdef DEBUG
    int32_t total_write_len;
#endif

    /* @todo Following condition should be atomic. Actually whole function should be atomic */
    uint8_t dma_memory_target = dma_current_memory_target(source->huart->hdmarx->Instance);
    uint16_t dma_remain_space = dma_current_data_counter(source->huart->hdmarx->Instance);

    /* Calculate correct write index - where we stop writting */
    if (dma_memory_target == 0)     // Writing in buffer 1
        write_index = source->buff_size - dma_remain_space;
    else                            // Writing in buffer 2
        write_index = source->buff_size * 2 - dma_remain_space;

    /* Unnecessary check @todo delete if never triggered */
    if (write_index > source->buff_size * 2) {
        bsp_error_handler(__FILE__, __LINE__, "Write index exceed limit. Logic error.");
        return 0;
    }

    /* Handle wrap around condition */
    if (write_index < source->read_index) {
        /* Last time we left in buffer 2 and now head is in buffer 1. Wrap around needed. */
#ifdef DEBUG
        total_write_len = source->buff_size * 2 - source->read_index + write_index;
#endif
        /* Read index points to where we left last time, so read till the end of second buffer */
        write_len = source->buff_size * 2 - source->read_index;
        if (write_len < 0) {
            bsp_error_handler(__FILE__, __LINE__, "Write length is negative.");
            return 0;
        }
        if (write_len != fifo_s_puts(source->data_fifo, &buff[source->read_index], write_len)) {
            bsp_error_handler(__FILE__, __LINE__, "FIFO overflow, need to resize.");
            return 0;
        }
        source->read_index = 0;
    }
    else {
        /* Head haven't wrap around yet. Simple direct write. */
#ifdef DEBUG
        total_write_len = write_index - source->read_index;
#endif
    }

#ifdef DEBUG
        BSP_DEBUG;
        print("Total write len: %d\r\n", total_write_len);
        print("Mem target:      %u\r\n", dma_memory_target);
        print("Mem remain:      %d\r\n", dma_remain_space);
        print("Write index:     %d\r\n", write_index);
        print("\r\n");
#endif

    /* General write condition */
    write_len = write_index - source->read_index;
    if (write_len != fifo_s_puts(source->data_fifo, &buff[source->read_index], write_len)) {
        bsp_error_handler(__FILE__, __LINE__, "FIFO overflow, need to resize.");
        return 0;
    }
    source->read_index = write_index;
    return 1;
}

uint8_t fifo_to_struct(data_process_t* source) {
    uint8_t byte = 0;
    uint16_t without_header_length = 0;
    uint16_t frame_length = 0;
    /* While FIFO is not empty */
    while (!fifo_is_empty(source->data_fifo)) {
        /* Get one byte */
        byte = fifo_s_get(source->data_fifo);
        /* Go over the rx decode step */
        switch (source->rx_step) {
            case STEP_HEADER_SOF:
                /* If we are at the first byte */
                if (byte == source->sof) {
                    source->frame_packet[source->frame_index] = byte;
                    source->frame_index++;
                    source->rx_step = STEP_DATA_LENGTH;
                }
                /* Idle spin till fifo empty. Cannot return. */
                else {
                    /* @todo Add debug option to check how many idle spin here */
                    /* @todo every time enter here should be error condition */
                    source->frame_index = 0;
                }
                break;
            case STEP_DATA_LENGTH:
                source->data_len = byte;
                source->frame_packet[source->frame_index] = byte;
                source->frame_index++;
                /* DATA LENGTH is uint16_t, so read one more byte */
                byte = fifo_s_get(source->data_fifo);
                source->data_len |= (byte << 8);
                source->frame_packet[source->frame_index] = byte;
                source->frame_index++;
                /* Check if data_len is valid */
                if (source->data_len < DATA_PROCESS_MAX_DATA_LEN) {
                    source->rx_step = STEP_FRAME_SEQ;
                }
                /* If data_len is not valid, go back to first step */
                else {
                    source->frame_index = 0;
                    source->rx_step = STEP_HEADER_SOF;
                }
                break;
            case STEP_FRAME_SEQ:
                source->frame_packet[source->frame_index] = byte;
                source->frame_index++;
                source->rx_step = STEP_CRC8;
                break;
            case STEP_CRC8:
                source->frame_packet[source->frame_index] = byte;
                source->frame_index++;
                /* If header CRC8 is correct, proceed to CRC16 check */
                if (verify_crc8_check_sum(source->frame_packet, DATA_PROCESS_HEADER_LEN)) {
                    source->rx_step = STEP_CRC16;
                }
                /* Otherwise go back to first step, idle spin */
                else {
                    source->frame_index = 0;
                    source->rx_step = STEP_HEADER_SOF;
                }
                break;
            case STEP_CRC16:
                without_header_length = DATA_PROCESS_CMD_LEN + source->data_len + DATA_PROCESS_CRC16_LEN;
                frame_length = DATA_PROCESS_HEADER_LEN + without_header_length;
                /* @todo Logic check. Remove after make sure ok */
                if (source->frame_index != DATA_PROCESS_HEADER_LEN) {
                    bsp_error_handler(__FILE__, __LINE__, "FRAME INDEX ERROR.");
                    return 0;
                }
                /* Get remaining data */
                if (without_header_length != fifo_s_gets(source->data_fifo, (uint8_t*)(source->frame_packet + source->frame_index), without_header_length)) {
                    bsp_error_handler(__FILE__, __LINE__, "Failed to get enough FIFO data.");
                    return 0;
                }
                source->frame_index = 0;
                source->rx_step = STEP_HEADER_SOF;
                /* Perform CRC16 check, if valid go to dispatcher */
                if (verify_crc16_check_sum(source->frame_packet, frame_length)) {
                    source->dispatcher(source->source_struct, source->frame_packet);
                }
                break;
            default:
                source->frame_index = 0;
                source->rx_step = STEP_HEADER_SOF;
                break;
        }
    }
    return 1;
}
