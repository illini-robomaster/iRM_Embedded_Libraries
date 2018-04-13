/******************************************************************************
	Description
		LED Utility
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/

#include "led.h"

/******************************************************************************
	Input
	Output
	Description
		Turn on RED LED
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_LED_RedOn(void) {
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_RESET);
}

/******************************************************************************
	Input
	Output
	Description
		Turn off RED LED
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_LED_RedOff(void) {
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_SET);
}

/******************************************************************************
	Input
	Output
	Description
		Toggle RED LED
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_LED_RedToggle(void) {
    HAL_GPIO_TogglePin(GPIOE, LED_RED_Pin);
}

/******************************************************************************
	Input
	Output
	Description
		Turn on GREEN LED
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_LED_GreenOn(void) {
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_RESET);
}

/******************************************************************************
	Input
	Output
	Description
		Turn off GREEN LED
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_LED_GreenOff(void) {
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_SET);
}

/******************************************************************************
	Input
	Output
	Description
		Toggle GREEN LED
	Log
		12/05/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_LED_GreenToggle(void) {
    HAL_GPIO_TogglePin(GPIOF, LED_GREEN_Pin);
}
