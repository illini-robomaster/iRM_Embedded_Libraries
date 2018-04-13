/******************************************************************************
	Description
		Laser Utility
	Log
		01/27/18 Nickel Liang	First Draft
*******************************************************************************/

#include "laser.h"

/******************************************************************************
	Input
	Output
	Description
		Turn on Laser
	Log
		01/27/18 Nickel Liang	First Draft
*******************************************************************************/
void RM_LASER_On(void) {
    HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET);
}

/******************************************************************************
	Input
	Output
	Description
		Turn on Laser
	Log
		01/27/18 Nickel Liang	First Draft
*******************************************************************************/
void RM_LASER_Off(void) {
    HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_RESET);
}
