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
 * @date    2018-05-27
 * @file    test_key.h
 * @brief   Test bsp_key.c/h
 * @log     2018-05-27 nickelliang
 */

#ifndef _TEST_KEY_H_
#define _TEST_KEY_H_

#include "bsp_key.h"

/**
 * Wait until a key pressed.
 *
 * @author Nickel_Liang
 * @date   2018-05-27
 */
uint8_t test_key(void);

#endif
