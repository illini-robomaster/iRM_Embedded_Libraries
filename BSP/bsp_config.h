/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-16
 * @file    bsp_config.h
 * @brief   Configure BSP ports
 * @log     2018-04-16 nickelliang
 */

#ifndef _BSP_CONFIG_H_
#define _BSP_CONFIG_H_

#include "stm32f4xx_hal.h"
#include "usart.h"

/**
 * @ingroup bsp
 * @defgroup bsp_config BSP CONFIG
 * @{
 */

/* This port require solder pins onto the board */
#define UART_FREE_PORT      huart2

#define BSP_DBUS_PORT       huart1
#define BSP_PRINT_PORT      huart6
#define BSP_TX2_PORT        huart3
#define BSP_REFEREE_PORT    UART_FREE_PORT

/* Choose where to print */
#define PRINT_TO_UART       1   // Set to 1 to print to uart port specified above
#define PRINT_TO_SD         0   // Not supported yet

/** @} */

#endif
