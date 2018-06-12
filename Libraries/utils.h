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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

#define sign(x) x < 0 ? -1 : (x == 0 ? 0 : 1)

#define min(x, y) x < y ? x : y
#define max(x, y) x > y ? x : y

/**
 * @brief clip int32_t number into [-lim, lim]
 * @param data pointer to the number to be clipped
 * @param lim  absolute limit range
 * @return clipped value
 */
int32_t abs_limit(int32_t *data, int32_t lim);

/**
 * @brief clip floating point number into [-lim, lim]
 * @param data pointer to the number to be clipped
 * @param lim  absolute limit range
 * @return clipped value
 */
float fabs_limit(float *data, float lim);

int32_t clip_to_range(int32_t *data, int32_t low_lim, int32_t high_lim);

float fclip_to_range(float *data, float low_lim, float high_lim);
    
#endif
