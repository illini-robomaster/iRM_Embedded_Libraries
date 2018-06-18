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
 * @author  Eric_Liang <hanghang177>
 * @date    2018-04-20
 * @file    test_servo.c
 * @brief   Servo lib test
 * #log     2018-04-20 hanghang177
 */

#include "test_servo.h"
#include "servo.h"

void test_servo() {
    servo_t servo;
    size_t i;

    servo_init(&servo, &htim5, 1, 1500);

    servo_setmax(&servo, 120);
    servo_setmin(&servo, 0);

    if (1) {
        for (i = 0; i <= 120; i+=10) {
            servo_write(&servo, i);
            HAL_Delay(1000);
        }
        servo_writeMicroseconds(&servo, 1500);
    }
}

