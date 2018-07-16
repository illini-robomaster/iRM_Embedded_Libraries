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
 * @file    dbus.h
 * @brief   DBUS utility
 * @log     2018-04-17 nickelliang
 */

#ifndef _DBUS_H_
#define _DBUS_H_

#include "bsp_error_handler.h"
#include "bsp_uart.h"
#include "bsp_config.h"
#include <string.h>
#include <stdlib.h>
#include "cmsis_os.h"

#define DBUS_BUF_LEN        18

typedef enum {
    RC_SWITCH_UP = 1,
    RC_SWITCH_MI = 3,
    RC_SWITCH_DN = 2,
} dbus_rc_switch_t;

typedef enum {
    RC_ROCKER_MIN = 364,
    RC_ROCKER_MID = 1024,
    RC_ROCKER_MAX = 1684,
    RC_ROCKER_ZERO_DRIFT = 10,   // Range of possible drift around initial position
    RC_ROCKER_MIN_MAX_DRIFT = RC_ROCKER_MAX - RC_ROCKER_MID + 10,   // Range of possible drift around min or max position
} dbus_rc_rocker_t;

typedef enum {
    MOUSE_MIN = -32768,
    MOUSE_STATIC = 0,
    MOUSE_MAX = 32767,
} dbus_mouse_t;

typedef struct {
    /* rocker channel information */
    int16_t ch0;    // S1*             *S2
    int16_t ch1;    //   C3-^       ^-C1
    int16_t ch2;    // C2-<   >+ -<   >+C0
    int16_t ch3;    //     +v       v+
    /* left and right switch information */
    uint8_t swl;
    uint8_t swr;
    /* mouse movement and button information */
    struct {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t l;
        uint8_t r;
    } __packed mouse;
    /* keyboard key information */
    union {
        uint16_t code;
        struct {
            uint16_t W:1;
            uint16_t S:1;
            uint16_t A:1;
            uint16_t D:1;
            uint16_t SHIFT:1;
            uint16_t CTRL:1;
            uint16_t Q:1;
            uint16_t E:1;
            uint16_t R:1;
            uint16_t F:1;
            uint16_t G:1;
            uint16_t Z:1;
            uint16_t X:1;
            uint16_t C:1;
            uint16_t V:1;
            uint16_t B:1;
        } __packed bit;
    } __packed key;
} __packed dbus_t;

/**
 * Initialize DBUS to DMA ready with interrupt disabled
 *
 * @return      1 for success, 0 for error
 * @author Nickel_Liang
 * @date   2018-04-17
 */
uint8_t dbus_init(void);

/**
 * Process DBUS data
 *
 * @param  buffer     Raw data buffer
 * @param  dbus       DBUS struct to be filled in
 * @return            1 for success, 0 for error
 * @author Nickel_Liang
 * @date   2018-04-18
 */
uint8_t dbus_data_process(uint8_t buffer[DBUS_BUF_LEN], dbus_t* dbus);

/**
 * Wrapper to get dbus struct
 *
 * @return            A dbus struct instance
 * @author Nickel_Liang
 * @date   2018-04-18
 */
dbus_t* dbus_get_struct(void);

#endif
