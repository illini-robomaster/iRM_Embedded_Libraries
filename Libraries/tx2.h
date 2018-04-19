/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    tx2.h
 * @brief   TX2 transmit / receive utility
 * @log     2018-04-18 nickelliang
 */

#ifndef _TX2_H_
#define _TX2_H_

#include "bsp_error_handler.h"
#include "bsp_uart.h"
#include "bsp_config.h"

/**
 * Initialize tx2 dma
 *
 * @return  1 for success, 0 for error
 * @author  Nickel_Liang
 * @date    2018-04-19
 * @note    Since TX2 tx and rx will take a relatively long period of time, consider put init / pack / unpack into a thread.
 */
uint8_t tx2_init(void);

#endif
