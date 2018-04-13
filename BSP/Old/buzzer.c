/******************************************************************************
	Description
		BUZZER Utility
	Log
		12/05/17 Nickel Liang	First Draft
        12/06/17 Nickel Liang   Add third volume option, Bug fix
*******************************************************************************/

#include "buzzer.h"

/******************************************************************************
	Input
	Output
	Description
		Initialize buzzer
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_BUZZER_Init(void) {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    TIM3->PSC = 83;
}

/******************************************************************************
	Input
        A tone
        Volume is 0 or 1. 1 is for test, smaller volume.
	Output
	Description
		Let Buzzer Sing a Tone
	Log
		12/05/17 Nickel Liang	First Draft
        12/06/17 Nickel Liang   Add third volume option
*******************************************************************************/
void RM_BUZZER_SingTone(RM_BUZZER_Freq freq, int volume) {
    TIM3->ARR = 1000000 / freq;     // Output Period
    if (volume == 0) {
        TIM3->CCR1 = TIM3->ARR / 2; // Output Volume
    } else if (volume == 1) {
        TIM3->CCR1 = TIM3->ARR / 100; // For Test
    } else {
        TIM3->CCR1 = TIM3->ARR / 200; // When I'm video chatting with my gf...
    }
}

/******************************************************************************
	Input
        A song
        Volume is 0 or 1. 1 is for test, smaller volume.
	Output
	Description
		Let Buzzer Sing a Song
	Log
		12/05/17 Nickel Liang	First Draft
        12/06/17 Nickel Liang   Bug Fix
*******************************************************************************/
void RM_BUZZER_SingSong(RM_BUZZER_Freq *freq, int volume) {
    int i = 0;
    while (freq[i] != Finish) {
        RM_BUZZER_SingTone(freq[i], volume);
        HAL_Delay(250);
        RM_BUZZER_SingTone(Silent, 0);
        HAL_Delay(50);
        i++;
    }
    RM_BUZZER_SingTone(Silent, 0);
}

/******************************************************************************
	Input
	Output
	Description
		Songs
	Log
		12/05/17 Nickel Liang	First Draft
        12/06/17 Nickel Liang   Little Star
*******************************************************************************/
RM_BUZZER_Freq Startup[] = {
    So5L, La6L, Mi3M, Silent, Finish
};

RM_BUZZER_Freq LittleStar[] = {
    Do1M, Do1M, So5M, So5M, La6M, La6M, So5M, Silent,
    Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Re2M, Do1M, Silent,
    So5M, So5M, Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Silent,
    So5M, So5M, Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Silent,
    Do1M, Do1M, So5M, So5M, La6M, La6M, So5M, Silent,
    Fa4M, Fa4M, Mi3M, Mi3M, Re2M, Re2M, Do1M, Silent, Finish
};
