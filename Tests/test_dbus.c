/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-17
 * @file    test_bsp_dbus.c
 * @brief   Test print for dbus value
 * @log     2018-04-17 nickelliang
 */

#include "test_dbus.h"

uint8_t test_dbus(void) {
    uint8_t ret = 1;
    size_t i;
    dbus_t* rc = dbus_get_struct();
    for (i = 0; i < DBUS_TEST_COUNT; i++) {
        print("CH0 %03d CH1 %03d ", rc->ch0, rc->ch1);
        print("CH2 %03d CH3 %03d ", rc->ch2, rc->ch3);
        print("SL %u SR %u ", rc->swl, rc->swr);
        print("MX %04d MY %04d ", rc->mouse.x, rc->mouse.y);
        print("ML %u MR %u ", rc->mouse.l, rc->mouse.r);
        if (rc->key.bit.SHIFT) print("SHIFT ");
        if (rc->key.bit.CTRL) print("CTRL ");
        if (rc->key.bit.W) print("W ");
        if (rc->key.bit.A) print("A ");
        if (rc->key.bit.S) print("S ");
        if (rc->key.bit.D) print("D ");
        if (rc->key.bit.Q) print("Q ");
        if (rc->key.bit.E) print("E ");
        if (rc->key.bit.R) print("R ");
        if (rc->key.bit.F) print("F ");
        if (rc->key.bit.G) print("G ");
        if (rc->key.bit.Z) print("Z ");
        if (rc->key.bit.X) print("X ");
        if (rc->key.bit.C) print("C ");
        if (rc->key.bit.V) print("V ");
        if (rc->key.bit.B) print("B ");
        print("\r\n");
    }
    return ret;
}
