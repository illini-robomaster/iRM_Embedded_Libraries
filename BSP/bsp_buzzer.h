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
 * @file    bsp_buzzer.h
 * @brief   Board support package for buzzer
 * @log     2018-04-15 nickelliang
 */

#ifndef _BSP_BUZZER_H_
#define _BSP_BUZZER_H_

#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#include "tim.h"

/**
 * @ingroup bsp
 * @defgroup bsp_buzzer BSP Buzzer
 * @{
 */

typedef enum{
    Do1L = 262,     ///*261.63Hz*/    3822us
    Re2L = 294,     ///*293.66Hz*/    3405us
    Mi3L = 330,     ///*329.63Hz*/    3034us
    Fa4L = 349,     ///*349.23Hz*/    2863us
    So5L = 392,     ///*392.00Hz*/    2551us
    La6L = 440,     ///*440.00Hz*/    2272us
    Si7L = 494,     ///*493.88Hz*/    2052us

    Do1M = 523,     ///*523.25Hz*/    1911us
    Re2M = 587,     ///*587.33Hz*/    1703us
    Mi3M = 659,     ///*659.26Hz*/    1517us
    Fa4M = 698,     ///*698.46Hz*/    1432us
    So5M = 784,     ///*784.00Hz*/    1276us
    La6M = 880,     ///*880.00Hz*/    1136us
    Si7M = 988,     ///*987.77Hz*/    1012us

    Do1H = 1047,     ///*1046.50Hz*/   956us
    Re2H = 1175,     ///*1174.66Hz*/   851us
    Mi3H = 1319,     ///*1318.51Hz*/   758us
    Fa4H = 1397,     ///*1396.91Hz*/   716us
    So5H = 1568,     ///*1567.98Hz*/   638us
    La6H = 1760,     ///*1760.00Hz*/   568us
    Si7H = 1976,     ///*1975.53Hz*/   506us

    Silent  = 0,
    Finish  = -1,
} buzzer_freq_t;

/**
 * Initialize the buzzer.
 *
 * @author Nickel_Liang
 * @date   2018-04-16
 */
void buzzer_init(void);

/**
 * Let buzzer sing a tone.
 *
 * @param  freq       A frequency or note in buzzer_freq_t
 * @author Nickel_Liang
 * @date   2018-04-16
 */
void buzzer_sing_tone(buzzer_freq_t freq);

/**
 * Let buzzer sing a sone.
 *
 * @param  freq       A song
 * @author Nickel_Liang
 * @date   2018-04-16
 * @note   This function CAN NOT be used in RTOS due to HAL_Delay
 */
void buzzer_sing_song(buzzer_freq_t *freq);

extern buzzer_freq_t startup[];
extern buzzer_freq_t initialize[];
extern buzzer_freq_t littleStar[];

/** @} */

#endif
