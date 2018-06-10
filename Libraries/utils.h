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

#define sign(x) x < 0 ? -1 : 1

#define min(x, y) x < y ? x : y
#define max(x, y) x > y ? x : y

inline int32_t abs_limit(int32_t *data, int32_t lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

inline float fabs_limit(float *data, float lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

#endif
