/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-27
 * @file    test_key.c
 * @brief   Test bsp_key.c/h
 * @log     2018-05-27 nickelliang
 */

#include "test_key.h"

uint8_t test_key(void) {
    print("Please press the key to continue...\r\n");
    while (!key_pressed_once());
    print("Key pressed!\r\n");
    return 1;
}
