/**
* @author  Nickel_Liang <nickelliang>
* @date    2018-04-15
* @file    bsp_buzzer.c
* @brief   Board support package for buzzer
* @log     2018-04-15 nickelliang
*/

#include "bsp_buzzer.h"

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

void buzzer_sing_sone(BUZZER_FREQ *freq, int volume) {
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

BUZZER_FREQ Startup[] = {
    So5L, La6L, Mi3M, Silent, Finish
};

BUZZER_FREQ LittleStar[] = {
    Do1M, Do1M, So5M, So5M, La6M, La6M, So5M, Silent,
    Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Re2M, Do1M, Silent,
    So5M, So5M, Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Silent,
    So5M, So5M, Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Silent,
    Do1M, Do1M, So5M, So5M, La6M, La6M, So5M, Silent,
    Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Re2M, Do1M, Silent, Finish
};
