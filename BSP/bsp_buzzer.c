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
* @file    bsp_buzzer.c
* @brief   Board support package for buzzer
* @log     2018-04-15 nickelliang
*/

#include "bsp_buzzer.h"

/* @todo Need to reconstruct to use standard PWM lib */

void buzzer_init(void) {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    TIM3->PSC = 83;
}

void buzzer_sing_tone(BUZZER_FREQ freq, int volume) {
    TIM3->ARR = 1000000 / freq;     // Output Period
    if (volume == 0) {
        TIM3->CCR1 = TIM3->ARR / 2; // Output Volume
    }
    else if (volume == 1) {
        TIM3->CCR1 = TIM3->ARR / 100; // For Test
    }
    else {
        TIM3->CCR1 = TIM3->ARR / 200; // When I'm video chatting with my gf...
    }
}

void buzzer_sing_song(BUZZER_FREQ *freq, int volume) {
    int i = 0;
    while (freq[i] != Finish) {
        buzzer_sing_tone(freq[i], volume);
        HAL_Delay(250);
        buzzer_sing_tone(Silent, 0);
        HAL_Delay(50);
        i++;
    }
    buzzer_sing_tone(Silent, 0);
}

BUZZER_FREQ startup[] = {
    So5L, La6L, Mi3M, Silent, Finish
};

BUZZER_FREQ initialize[] = {
    So5L, Finish
};

BUZZER_FREQ littleStar[] = {
    Do1M, Do1M, So5M, So5M, La6M, La6M, So5M, Silent,
    Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Re2M, Do1M, Silent,
    So5M, So5M, Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Silent,
    So5M, So5M, Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Silent,
    Do1M, Do1M, So5M, So5M, La6M, La6M, So5M, Silent,
    Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Re2M, Do1M, Silent, Finish
};
