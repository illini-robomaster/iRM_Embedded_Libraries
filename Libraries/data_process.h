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

/* Declare data_process_t */
typedef struct _data_process data_process_t;

/* Define dispatcher_func_t */
typedef uint8_t (*dispatcher_func_t)(void* target_struct, data_process_t* process_struct);

/* Define packer_func_t */
typedef uint8_t (*packer_func_t)(void* target_struct, data_process_t* process_struct, uint16_t cmdid);

/* Define data_process_t */
typedef struct _data_process {
    /* Commonly used */
    UART_HandleTypeDef *huart;  // Which UART data is comming from
    /* Used for incomming rx msg */
    fifo_s_t    *data_fifo;     // FIFO to store data into
    /* Used in buffer_to_fifo */
    uint8_t     *buff[2];       // Pointer to double buffer
    uint16_t    buff_size;      // Size of single buffer
    uint16_t    read_index;     // Where we left last time
    /* Used in fifo_to_struct */
    void        *source_struct; // Used by dispatcher. = target_struct
    dispatcher_func_t dispatcher_func;  // A dispatcjer function pointer
    uint8_t     sof;            // Start of frame
    uint8_t     *frame_packet;  // Contain one frame of data. Same length as fifo
    uint16_t    data_len;       // Store the data length got from header
    /* Used for outgoing tx msg */
    fifo_s_t    *transmit_fifo; // FIFO to store outgoing msg
    packer_func_t packer_func;  // A packer function pointer
} data_process_t;

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
 * @param  tx_mutex      Mutex to be used by tx FIFO
 * @param  packer_func   Function pointer to corresponding packer
 * @return               A data process instance, NULL if failed
 * @author Nickel_Liang
 * @date   2018-04-20
 */
data_process_t* data_process_init(UART_HandleTypeDef *huart, osMutexId mutex, uint32_t fifo_size, uint16_t buffer_size, uint8_t sof, dispatcher_func_t dispatcher_func, void *source_struct, osMutexId tx_mutex, packer_func_t packer_func);

/**
 * Perform a rx data process sequence
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
uint8_t data_process_rx(data_process_t* source);

/**
 * Perform a tx data process sequence
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-05-26
 */
uint8_t data_process_tx(data_process_t* source);

/**
 * Convert DMA double buffer to FIFO. This can improve data retrive efficiency
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-19
 */
static uint8_t buffer_to_fifo(data_process_t* source);

/**
 * Convert a FIFO to appropriate data structure
 *
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-20
 */
static uint8_t fifo_to_struct(data_process_t* source);

/**
 * Convert a data stream to FIFO
 *
 * @param  cmdid      Message CMDID
 * @param  data       Data stream
 * @param  length     length of data
 * @param  source     A valid data process structure
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-05-26
 */
uint8_t data_to_fifo(uint16_t cmdid, uint8_t *data, uint16_t length, data_process_t *source);

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

void print_buffer(data_process_t* source, uint8_t buffer_num);

#endif
