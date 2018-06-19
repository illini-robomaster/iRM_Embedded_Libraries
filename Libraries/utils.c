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

#include "utils.h"
#include <math.h>

int32_t abs_limit(int32_t *data, int32_t lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

float fabs_limit(float *data, float lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

int32_t clip_to_range(int32_t *data, int32_t low_lim, int32_t high_lim) {
    if (low_lim > high_lim)
        return *data;
    if (*data < low_lim)
        *data = low_lim;
    if (*data > high_lim)
        *data = high_lim;
    return *data;
}

float fclip_to_range(float *data, float low_lim, float high_lim) {
    if (low_lim > high_lim)
        return *data;
    if (*data < low_lim)
        *data = low_lim;
    if (*data > high_lim)
        *data = high_lim;
    return *data;
}

void normalize_2d(float *vx, float *vy) {
    float norm = sqrt(*vx * *vx + *vy * *vy);
    *vx /= norm;
    *vy /= norm;
}

