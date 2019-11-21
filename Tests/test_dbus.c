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
 * @date    2018-04-17
 * @file    test_bsp_dbus.c
 * @brief   Test print for dbus value
 * @log     2018-04-17 nickelliang
 */

#include "bsp_print.h"

#include "test_dbus.h"

uint8_t test_dbus(void) {
    uint8_t ret = 1;
    size_t i;
    dbus_t* rc = dbus_get_struct();
    for (i = 0; i < DBUS_TEST_COUNT; i++) {
        printf("CH0 %03d CH1 %03d ", rc->ch0, rc->ch1);
        printf("CH2 %03d CH3 %03d ", rc->ch2, rc->ch3);
        printf("SL %u SR %u ", rc->swl, rc->swr);
        printf("MX %04d MY %04d ", rc->mouse.x, rc->mouse.y);
        printf("ML %u MR %u ", rc->mouse.l, rc->mouse.r);
        if (rc->key.bit.SHIFT) printf("SHIFT ");
        if (rc->key.bit.CTRL) printf("CTRL ");
        if (rc->key.bit.W) printf("W ");
        if (rc->key.bit.A) printf("A ");
        if (rc->key.bit.S) printf("S ");
        if (rc->key.bit.D) printf("D ");
        if (rc->key.bit.Q) printf("Q ");
        if (rc->key.bit.E) printf("E ");
        if (rc->key.bit.R) printf("R ");
        if (rc->key.bit.F) printf("F ");
        if (rc->key.bit.G) printf("G ");
        if (rc->key.bit.Z) printf("Z ");
        if (rc->key.bit.X) printf("X ");
        if (rc->key.bit.C) printf("C ");
        if (rc->key.bit.V) printf("V ");
        if (rc->key.bit.B) printf("B ");
        printf("\r\n");
    }
    return ret;
}
