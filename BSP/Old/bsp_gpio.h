/******************************************************************************
	Description
		GPIO Utility
	Log
		01/03/18 Nickel Liang	First Draft
        01/15/18 Nickel Liang   Add 7-seg display test
*******************************************************************************/

#ifndef __BSP_GPIO
#define __BSP_GPIO

#include "mytype.h"

/*******************
GND     X X     5V
PF10    X X     PI9
PA5     X X     PA4
PC5  R  X X  W  PC1     LSB D
PC4  R  X X  W  PC0         C
PC3  R  X X  W  PB1         B
PC2  R  X X  W  PB0     MSB A
PE6     X X     PE1
PE5     X X     PE4
PF1 SCL X X SDA PF0
*******************/

#define RM_A_On  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)
#define RM_A_Off HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)
#define RM_B_On  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
#define RM_B_Off HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
#define RM_C_On  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define RM_C_Off HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define RM_D_On  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define RM_D_Off HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define RM_E_On  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET)
#define RM_E_Off HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)
#define RM_F_On  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET)
#define RM_F_Off HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)
#define RM_G_On  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET)
#define RM_G_Off HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET)

/* Global Functions */
void RM_GPIO_WriteBin(uint8_t num);
uint8_t RM_GPIO_ReadBin(void);
void RM_GPIO_DisplayDigit(uint8_t i);
void RM_GPIO_ClearDigit(void);
void RM_GPIO_DigitTest(void);

/* Debug Utility Functions */
void RM_DEBUG_GPIO_Test(void);

#endif
