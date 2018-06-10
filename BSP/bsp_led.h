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
 * @date    2018-04-14
 * @file    bsp_led.h
 * @brief   led board support package
 * @log     2018-04-14 nickelliang
 */

#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#include "stm32f4xx_hal.h"
#include "main.h"

/**
 * @ingroup bsp
 * @defgroup bsp_led BSP led
 * @{
 */

/**
 * Turn on red led
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void led_red_on(void);

/**
 * Turn off red led
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void led_red_off(void);

/**
 * Turn on green led
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void led_green_on(void);

/**
 * Turn off green led
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void led_green_off(void);

/**
 * Turn on all led
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void led_on(void);

/**
 * Turn off all led
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void led_off(void);

/** @} */

#endif
