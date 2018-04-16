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
} BUZZER_FREQ;

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
 * @param  freq       A frequency or note in BUZZER_FREQ
 * @param  volume     Volume of the buzzer
 * @author Nickel_Liang
 * @date   2018-04-16
 */
void buzzer_sing_tone(BUZZER_FREQ freq, int volume);

/**
 * Let buzzer sing a sone.
 *
 * @param  freq       A song
 * @param  volume     Volume of the buzzer
 * @author Nickel_Liang
 * @date   2018-04-16
 * @note   This function CAN NOT be used in RTOS due to HAL_Delay
 */
void buzzer_sing_song(BUZZER_FREQ *freq, int volume);

extern BUZZER_FREQ startup[];
extern BUZZER_FREQ initialize[];
extern BUZZER_FREQ littleStar[];

/** @} */

#endif
