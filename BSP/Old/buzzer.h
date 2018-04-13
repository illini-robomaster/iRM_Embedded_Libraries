/******************************************************************************
	Description
		BUZZER Utility
	Log
		12/05/17 Nickel Liang	First Draft
        12/06/17 Nickel Liang	Bug Fix
*******************************************************************************/

#ifndef __BUZZER
#define __BUZZER

#include "mytype.h"

/* Buzzer Frequency */
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
} RM_BUZZER_Freq;

/* Global Functions */
void RM_BUZZER_Init(void);
void RM_BUZZER_SingTone(RM_BUZZER_Freq freq, int volume);
void RM_BUZZER_SingSong(RM_BUZZER_Freq *freq, int volume);

/* Song */
extern RM_BUZZER_Freq Startup[];
extern RM_BUZZER_Freq LittleStar[];

#endif
