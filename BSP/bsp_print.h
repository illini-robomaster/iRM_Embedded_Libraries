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
 * @defgroup bsp_print BSP Print
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
