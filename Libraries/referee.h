/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    referee.h
 * @brief   Referee system utility
 * @log     2018-04-18 nickelliang
 */

#ifndef _REFEREE_H_
#define _REFEREE_H_

#include "bsp_error_handler.h"
#include "bsp_uart.h"
#include "bsp_config.h"

/**
 * Initialize referee system receive
 *
 * @return            1 for success, 0 for error
 * @author Nickel_Liang
 * @date   2018-04-19
 * @note   Since referee system tx and rx will take a relatively long period of time, consider put init / pack / unpack into a thread.
 */
uint8_t referee_init(void);

#endif
