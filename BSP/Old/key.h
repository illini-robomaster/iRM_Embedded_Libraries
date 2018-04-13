/******************************************************************************
	Description
		KEY Utility
	Log
		12/05/17 Nickel Liang	First Draft
        12/07/17 Nickel Liang   Easier to use
*******************************************************************************/

#ifndef __KEY
#define __KEY

#include "mytype.h"

/* Define */
#define RM_KEY_Pressed !HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) // Read val is 0
#define RM_KEY_Released HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)

#endif
