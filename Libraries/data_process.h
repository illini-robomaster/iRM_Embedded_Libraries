/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-19
 * @file    data_process.h
 * @brief   Process uart received data in DMA buffer
 * @log     2018-04-19 nickelliang
 */

#ifndef _DATA_PROCESS_H_
#define _DATA_PROCESS_H_

#include "data_fifo.h"
#include "crc_check.h"
#include "bsp_error_handler.h"
#include "bsp_uart.h"
#include "cmsis_os.h"
#include <stdlib.h>

#define DATA_PROCESS_HEADER_LEN     5
#define DATA_PROCESS_CMD_LEN        2
#define DATA_PROCESS_CRC16_LEN      2

#define DATA_PROCESS_MAX_FRAME_LEN  256
#define DATA_PROCESS_MAX_DATA_LEN   (DATA_PROCESS_MAX_FRAME_LEN - DATA_PROCESS_HEADER_LEN - DATA_PROCESS_CMD_LEN - DATA_PROCESS_CRC16_LEN)

/**
 * Package frame format:
 *  FrameHeader 5B [SOF 1B + Data Length 2B + SEQ 1B + CRC8 1B]
 *  CmdID       2B
 *  Data        nB
 *  FrameTail   2B CRC16
 */

typedef struct {
    /* Commonly used */
    UART_HandleTypeDef* huart;  // Which UART data is comming from
    fifo_s_t*   data_fifo;      // FIFO to store data into
    /* Used in buffer_to_fifo */
    uint8_t*    buff[2];        // Pointer to double buffer
    uint16_t    buff_size;      // Size of single buffer
    uint16_t    read_index;     // Where we left last time
    /* Used in fifo_to_struct */
    void*       source_struct;  // Used by dispatcher. = target_struct
    uint8_t     (*dispatcher)(void* target_struct, uint8_t* buffer);
    uint8_t     sof;            // Start of frame
    uint8_t*    frame_packet;   // Contain one frame of data. Same length as fifo
    uint16_t    data_len;       // Store the data length got from header
} data_process_t;
/* I regreted. I really should use C++ to write our lib... */

/**
 * Initialize a data process instance for a UART port
 *
 * @param  huart         Which UART port to process
 * @param  mutex         Mutex to be used by FIFO
 * @param  fifo_size     Size of FIFO
 * @param  buffer_size   Size of DMA buffer
 * @param  sof           SOF of UART
 * @param  dispatcher    Function pointer to appropriate dispatcher
 * @param  source_struct Struct of the source
 * @return               A data process instance
 * @author Nickel_Liang
 * @date   2018-04-20
 */
data_process_t* data_process_init(UART_HandleTypeDef* huart, osMutexId mutex, uint32_t fifo_size, uint16_t buffer_size, uint8_t sof, uint8_t(*dispatcher)(void *, uint8_t *), void* source_struct);

/**
 * Convert DMA double buffer to FIFO. This can improve data retrive efficiency
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-19
 */
uint8_t buffer_to_fifo(data_process_t* source);

/**
 * Convert a FIFO to appropriate data structure
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-20
 */
uint8_t fifo_to_struct(data_process_t* source);

/**
 * Process a data header and perform CRC8 check
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t process_header(data_process_t* source);

/**
 * Process a data frame and perfrom CRC16 check
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t process_frame(data_process_t* source);

#endif
