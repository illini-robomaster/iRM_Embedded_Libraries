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
 * @date    2018-04-15
 * @file    bsp_laser.h
 * @brief   Board support package for laser
 * @log     2018-04-15 nickelliang
 */

#ifndef _BSP_LASER_H_
#define _BSP_LASER_H_

#include "stm32f4xx_hal.h"
#include "main.h"

/**
 * @ingroup bsp
 * @defgroup bsp_laser BSP Laser
 * @{
 */

/**
 * Turn on laser
 *
 * @author Nickel_Liang
 * @date   2018-04-15
 */
void laser_on(void);

/**
 * Turn off laser
 *
 * @author Nickel_Liang
 * @date   2018-04-15
 */
void laser_off(void);

/** @} */

#endif
