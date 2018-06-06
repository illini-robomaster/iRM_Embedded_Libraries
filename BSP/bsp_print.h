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

/**
 * Implementation of printf equivalent in embedded system
 *
 * @param  fmt     formatted string
 * @param  ...     variable length arguments 
 * @author Yixiao Sun
 * @date   2017-12-20
 */
void print(const char *fmt, ...);

/** @} */

#endif // _BSP_PRINT_
