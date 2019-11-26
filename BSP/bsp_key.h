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
 * @date    2018-04-15
 * @file    bsp_key.h
 * @brief   Key press board support package
 * @log     2018-05-27 nickelliang
 */

#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "main.h"

/**
 * @ingroup bsp
 * @defgroup bsp_key BSP Key
 * @{
 */

#define KEY_PRESSED     1
#define KEY_DEBOUNCE_MS 25
// #define KEY_RESET_MS    3000

typedef enum {
    KEY_UP,
    KEY_DOWN,
} key_state_t;

/**
 * Initilaize key press condition
 *
 * @author Nickel_Liang
 * @date   2018-05-27
 */
void key_init(void);

/**
 * Key press finite state machine
 *
 * @author Nickel_Liang
 * @date   2018-05-27
 */
void key_fsm(void);

/**
 * Check if a key press is valid
 *
 * @return            [description]
 * @author Nickel_Liang
 * @date   2018-05-27
 */
static uint8_t key_valid(void);

/**
 * Check if key is pressed. This function will countinuously return 1 until key is released.
 *
 * @return            1 for key press, 0 for key release
 * @author Nickel_Liang
 * @date   2018-05-27
 */
uint8_t key_pressed(void);

/**
 * Return how many times the key has been pressed.
 *
 * @return            # of key press since startup
 * @author Nickel_Liang
 * @date   2018-05-27
 */
uint8_t key_press_count(void);

/**
 * Interface to query if key pressed once or more
 *
 * @return            0 if didn't press since last query
 *                    1 if pressed at least once since last query
 * @author Nickel_Liang
 * @date   2018-05-27
 */
uint8_t key_pressed_once(void);

/**
 * Interface to query how many times the key has been pressed since last query. If you want to use in a condition, key_pressed_once is recommanded.
 *
 * @return            # of press since last query
 * @author Nickel_Liang
 * @date   2018-05-27
 */
uint16_t key_pressed_time(void);

/**
 * Use blocking method to wait for a key press.
 *
 * @author Nickel_Liang
 * @date   2018-05-27
 */
void wait_until_key_pressed(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
