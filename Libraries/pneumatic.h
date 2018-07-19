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

#ifndef PNEUMATIC_H_
#define PNEUMATIC_H_

#ifdef HERO

#include "bsp_pneumatic.h"

void grabber_grab(void);

void grabber_release(void);

void extend_grabber_mechanics(void);

void reset_grabber_mechanics(void);

void front_legs_extend(void);

void front_legs_reset(void);

void rear_legs_extend(void);

void rear_legs_reset(void);

#endif

#endif
