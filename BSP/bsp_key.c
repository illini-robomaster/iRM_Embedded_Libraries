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
 * @date    2018-05-27
 * @file    bsp_key.c
 * @brief   BSP for key press
 * @log     2018-05-27 nickelliang
 */

#include "bsp_key.h"

static uint32_t key_previous_tick;
uint16_t        key_previous_count;
key_state_t     key_state;
uint8_t         key_press;
uint16_t        key_count;

void key_init(void) {
    key_count = 0;
    key_previous_count = 0;
    key_state = KEY_UP;
    key_previous_tick = HAL_GetTick();
    key_press = !KEY_PRESSED;
}

void key_fsm(void) {
    if (key_valid()) {
        switch (key_state) {
            case KEY_UP:
                key_count++;
                key_press = KEY_PRESSED;
                key_state = KEY_DOWN;
                break;
            case KEY_DOWN:
                key_press = !KEY_PRESSED;
                key_state = KEY_UP;
                break;
        }
    }
}

static inline uint8_t key_valid(void) {
    uint32_t tick = HAL_GetTick();
    uint8_t ret = 0;
    // if (tick - key_previous_tick > KEY_RESET_MS) {
    //     key_press = !KEY_PRESSED;
    //     ret = 1;
    // }
    if (tick - key_previous_tick > KEY_DEBOUNCE_MS)
        ret = 1;
    key_previous_tick = tick;
    return ret;
}

uint8_t key_pressed(void) {
    return key_press;
}

uint8_t key_press_count(void) {
    return key_count;
}

uint8_t key_pressed_once(void) {
    if (key_previous_count == key_count)
        return 0;
    key_previous_count = key_count;
    return 1;
}

uint16_t key_pressed_time(void) {
    uint16_t diff = key_count - key_previous_count;
    key_previous_count = key_count;
    return diff;
}

void wait_until_key_pressed(void) {
    print("Please press the key to continue...\r\n");
    while (!key_pressed_once());
}
