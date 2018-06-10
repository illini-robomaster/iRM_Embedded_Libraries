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

#include "test_referee.h"

void test_referee(void) {
    while (1) {
        print("chassis power: %5.2f ", referee_info.power_heat_data.chassis_power);
        print("chassis current: %5.2f ", referee_info.power_heat_data.chassis_current);
        print("chassis voltage: %5.2f ", referee_info.power_heat_data.chassis_volt);
        print("busrt buffer: %5.2f\r\n", referee_info.power_heat_data.chassis_pwr_buf);
        osDelay(50);
    }
}
