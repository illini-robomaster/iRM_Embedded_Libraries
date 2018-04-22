/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-15
 * @file    bsp_error_handler.h
 * @brief   Handle error conditions.
 * @log     2018-04-15 nickelliang
 */

#ifndef _BSP_ERROR_HANDLER_H_
#define _BSP_ERROR_HANDLER_H_

#include "bsp_print.h"

/**
 * @ingroup bsp
 * @defgroup bsp_error_handler BSP Error Handler
 * @{
 */

#define BSP_DEBUG   print("[DEBUG] %s:%d ", __FUNCTION__, __LINE__)

/**
 * Handle error condition printf etc.
 *
 * @param  file       Which file the error occured
 * @param  line       Which line the error occured
 * @param  msg        Message want to print
 * @author Nickel_Liang
 * @date   2018-04-15
 */
extern void bsp_error_handler(const char* func, int line, char* msg);

/** @} */

#endif
