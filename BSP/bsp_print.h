/**
 * @author  Yixiao Sun
 * @date    2017-12-20
 * @file    bsp_print.h
 * @brief
 * @log     2018-04-16 nickelliang
 */

#ifndef _BSP_PRINT_H_
#define _BSP_PRINT_H_

#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#include <string.h>

/**
 * @ingroup bsp
 * @defgroup bsp_print BSP PRINT
 * @{
 */

static char printf_temp[512];

/**
 * Implementation of printf equivalent in embedded system
 *
 * @param  expr     string and format to print
 * @author Yixiao Sun
 * @date   2017-12-20
 */
#define print(expr...) do {                                                      \
    sprintf(printf_temp, expr);                                                  \
    if (PRINT_TO_UART == 1)                                                      \
        HAL_UART_Transmit(&BSP_PRINT_PORT, printf_temp, strlen(printf_temp), 2); \
    if (PRINT_TO_SD == 1)                                                        \
        break;                                                                   \
} while(0)

/** @} */

#endif // _BSP_PRINT_
