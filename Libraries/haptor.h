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

#ifndef _HAPTOR_H_
#define _HAPTOR_H_

#include "motor.h"
#include "pid.h"
#include <inttypes.h>

/**
 * @enum    haptor_state_t
 * @var TOP     the state when haptor reaches its upper limit
 * @var BOTTOM  the state when haptor reaches its lower limit
 * @var MIDDLE  the state when haptor is in between its two limits
 */
typedef enum {
    TOP,
    MIDDLE,
    BOTTOM,
}   haptor_state_t;

/**
 * @brief initialize haptor for engineering bot
 */
void init_haptor();

/**
 * @brief lower the haptor
 * @note the haptor will be controlled by a speed / speed pid and
 *       it will stop when it hits anything
 */
void haptor_down();

/**
 * @brief lift the haptor
 * @note the haptor will be controlled by a speed / speed pid and
 *       it will stop when it hits anything
 */
void haptor_up();

#endif
