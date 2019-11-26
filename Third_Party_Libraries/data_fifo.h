/**************************************************************************
 *  Copyright (C) 2018 RoboMaster.
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
 * @author  RoboMaster
 * @date    2017-06
 * @file    data_fifo.h
 * @brief   Genernal FIFO model interface for any data element type.
 * @log     2018-04-18 nickelliang
 * @note    This is a circular buffer implementation of FIFO(Queue)
 */

#ifndef _DATA_FIFO_H_
#define _DATA_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <stdlib.h>

#define ASSERT(x) do {                          \
    while(!(x));                                \
} while(0)                                      \

#define MUTEX_WAIT() do {                       \
    osMutexWait(pfifo->mutex, osWaitForever);   \
} while(0)                                      \

#define MUTEX_RELEASE() do {                    \
    osMutexRelease(pfifo->mutex);               \
} while(0)                                      \

//! FIFO Memory Model (Single Byte Mode)
typedef struct {
    uint8_t   *start_addr;      // Start Address
    uint8_t   *end_addr;        // End Address
    uint32_t  free;             // The capacity of FIFO
    uint32_t  buf_size;         // Buffer size
    uint32_t  used;             // The number of elements in FIFO
    uint8_t   read_index;       // Read Index Pointer
    uint8_t   write_index;      // Write Index Pointer
    osMutexId mutex;
} fifo_s_t;

/**
 * Create a FIFO
 *
 * @param  unit_cnt   # of bytes to allocate
 * @param  mutex      A mutex previously initialized
 * @return            A fifo_s_t if success, NULL if failed
 */
fifo_s_t* fifo_s_create(uint32_t unit_cnt, osMutexId mutex);

/**
 * Destroy a FIFO
 *
 * @param  pfifo      FIFO to be destroyed
 */
void fifo_s_destory(fifo_s_t* pfifo);

/**
 * Initialize FIFO structure
 *
 * @param  pfifo      FIFO to be init
 * @param  base_addr  Base addr of FIFO
 * @param  unit_cnt   # of bytes allocated
 * @param  mutex      FIFO mutex
 * @return            -1 if mutex is invalid, 0 if success
 */
int32_t fifo_s_init(fifo_s_t* pfifo, void* base_addr, uint32_t unit_cnt, osMutexId mutex);

/**
 * Put an element into FIFO
 *
 * @param  pfifo      Pointer of valid FIFO
 * @param  element    Data element you want to put
 * @return            0 if success, -1 if FIFO is full
 */
int32_t fifo_s_put(fifo_s_t* pfifo, uint8_t element);

/**
 * Put some elements into FIFO
 *
 * @param  pfifo      Pointer of valid FIFO
 * @param  psource    Data elements you want to put
 * @param  number     Number of data elements
 * @return            -1 if something wrong, other num indicate how many elements successfully put into FIFO
 */
int32_t fifo_s_puts(fifo_s_t *pfifo, uint8_t *psource, uint32_t number);

/**
 * Get an element from FIFO
 *
 * @param  pfifo      Pointer of valid FIFO
 * @return            Data element poped from FIFO
 */
uint8_t fifo_s_get(fifo_s_t* pfifo);

/**
 * Get elements from FIFO
 *
 * @param  pfifo      Pointer of a valid FIFO
 * @param  source     Where to store poped elements
 * @param  len        How many elements to retrive
 * @return            Actual number of elements retrived
 */
uint16_t fifo_s_gets(fifo_s_t* pfifo, uint8_t* source, uint8_t len);

/**
 * Peek an element value at specific location
 *
 * @param  pfifo      Pointer of a valid FIFO
 * @param  offset     Offset from current pointer
 * @return            Data at offset location
 */
uint8_t fifo_s_peek(fifo_s_t* pfifo, uint8_t offset);

/**
 * Check if FIFO is empty
 *
 * @param  pfifo      Pointer of a valid FIFO
 * @return            1 if empty, 0 otherwise
 */
uint8_t fifo_is_empty(fifo_s_t* pfifo);

/**
 * Check if FIFO is full
 *
 * @param  pfifo      Pointer of a valid FIFO
 * @return            1 if full, 0 otherwise
 */
uint8_t fifo_is_full(fifo_s_t* pfifo);

/**
 * Check how many entries are filled
 *
 * @param  pfifo      Pointer of a valid FIFO
 * @return            # of used location
 */
uint32_t fifo_used_count(fifo_s_t* pfifo);

/**
 * Check how many entries are free
 *
 * @param  pfifo      Pointer of a valid FIFO
 * @return            # of free location
 */
uint32_t fifo_free_count(fifo_s_t* pfifo);

/**
 * Flush the FIFO
 *
 * @param  pfifo      Pointer of a valid FIFO
 * @return            0
 * @note    Previous content still persist
 */
uint8_t fifo_flush(fifo_s_t* pfifo);

#ifdef __cplusplus
}
#endif

#endif
