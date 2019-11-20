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
 * @date    2018-04-20
 * @file    test_bsp_imu.h
 * @brief   Test imu functionality
 * @log     2018-04-20 nickelliang
 */

#ifndef _TEST_BSP_IMU_H_
#define _TEST_BSP_IMU_H_

#include "bsp_imu.h"
#include "imu_onboard.h"

#define IMU_TEST_COUNT     100000

uint8_t test_bsp_imu(void);

#endif
