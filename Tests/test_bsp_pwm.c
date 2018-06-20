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

#include "test_bsp_pwm.h"
#include "cmsis_os.h"

void test_bsp_pwm() {
    pwm_t snail_left, snail_right;
    pwm_init(&snail_left, &htim12, 2);
    pwm_init(&snail_right, &htim12, 1);

    osDelay(4000);
    
    while (1) {
        pwm_set_pulse_width(&snail_left, 1280);
        pwm_set_pulse_width(&snail_right, 1280);
        osDelay(10000);
}
}
