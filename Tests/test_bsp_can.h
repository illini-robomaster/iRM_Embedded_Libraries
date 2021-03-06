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
 * @date    2018-04-17
 * @file    test_bsp_can.h
 * @brief   Test print for can value
 * @log     2018-04-17 nickelliang
 */

#ifndef _TEST_BSP_CAN_H_
#define _TEST_BSP_CAN_H_

#include "bsp_can.h"
#include "bsp_print.h"

#define CAN_TEST_COUNT      100000
#define PRINT_CAN_1         1       // Set to 1 to print can 1 data
#define PRINT_CAN_2         0       // Set to 1 to print can 2 data

uint8_t test_bsp_can(void);

#endif
