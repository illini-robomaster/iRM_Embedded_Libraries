/******************************************************************************
	Description
		GPIO Utility
	Log
		01/03/18 Nickel Liang	First Draft
        01/15/18 Nickel Liang   Add 7-seg display test
*******************************************************************************/

#include "bsp_gpio.h"

/******************************************************************************
	Input
        num: number in range 1~15
	Output
	Description
		Set GPIO Pins
	Log
		01/03/18 Nickel Liang	First Draft
*******************************************************************************/
void RM_GPIO_WriteBin(uint8_t num) {
    if (num & 0x01) { //PC1 LSB
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //1
    } else {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //0
    }
    if (num & 0x02) { //PC0
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
    }
    if (num & 0x04) { //PB1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
    }
    if (num & 0x08) { //PB0 HSB
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    }
}

/******************************************************************************
	Input
	Output
        number from gpio pins
	Description
		Read GPIO Pins
	Log
		01/03/18 Nickel Liang	First Draft
*******************************************************************************/
uint8_t RM_GPIO_ReadBin(void) {
    uint8_t num = 0;
    num |= ((uint8_t)HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5));
    num |= ((uint8_t)HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4)<<1);
    num |= ((uint8_t)HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)<<2);
    num |= ((uint8_t)HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)<<3);
    return num;
}

/******************************************************************************
	Input
	Output
	Description
		Go over GPIO test loop. Connect:
        PC5 - PC1
        PC4 - PC0
        PC3 - PB1
        PC2 - PB0
	Log
		01/03/18 Nickel Liang	First Draft
*******************************************************************************/
void RM_DEBUG_GPIO_Test(void) {
    uint8_t i, receive, flag = 0;
    RM_PRINTF("*****GPIO TX/RX TEST*****\r\n");
    for (i = 0; i < 16; i++) {
        RM_GPIO_WriteBin(i);
        receive = RM_GPIO_ReadBin();
        RM_PRINTF("TX %02d RX %02d\r\n", i, receive);
        if (i != receive) {
            flag = 1;
        }
    }
    if (flag == 1) {
        RM_PRINTF("GPIO TEST FAILED. CHECK CONNECTION.\r\n");
    } else {
        RM_PRINTF("GPIO TEST PASSED.\r\n");
    }
    RM_PRINTF("*************************\r\n");
}

/******************************************************************************
	Input
        Digit to print
	Output
	Description
        Print a digit via 7-seg display
	Log
		01/15/18 Nickel Liang	First Draft
*******************************************************************************/
void RM_GPIO_DisplayDigit(uint8_t i) {
    switch (i) {
        case 0:
            RM_A_On;
            RM_B_On;
            RM_C_On;
            RM_D_On;
            RM_E_On;
            RM_F_On;
            RM_G_Off;
            break;
        case 1:
            RM_A_Off;
            RM_B_On;
            RM_C_On;
            RM_D_Off;
            RM_E_Off;
            RM_F_Off;
            RM_G_Off;
            break;
        case 2:
            RM_A_On;
            RM_B_On;
            RM_C_Off;
            RM_D_On;
            RM_E_On;
            RM_F_Off;
            RM_G_On;
            break;
        case 3:
            RM_A_On;
            RM_B_On;
            RM_C_On;
            RM_D_On;
            RM_E_Off;
            RM_F_Off;
            RM_G_On;
            break;
        case 4:
            RM_A_Off;
            RM_B_On;
            RM_C_On;
            RM_D_Off;
            RM_E_Off;
            RM_F_On;
            RM_G_On;
            break;
        case 5:
            RM_A_On;
            RM_B_Off;
            RM_C_On;
            RM_D_On;
            RM_E_Off;
            RM_F_On;
            RM_G_On;
            break;
        case 6:
            RM_A_On;
            RM_B_Off;
            RM_C_On;
            RM_D_On;
            RM_E_On;
            RM_F_On;
            RM_G_On;
            break;
        case 7:
            RM_A_On;
            RM_B_On;
            RM_C_On;
            RM_D_Off;
            RM_E_Off;
            RM_F_Off;
            RM_G_Off;
            break;
        case 8:
            RM_A_On;
            RM_B_On;
            RM_C_On;
            RM_D_On;
            RM_E_On;
            RM_F_On;
            RM_G_On;
            break;
        case 9:
            RM_A_On;
            RM_B_On;
            RM_C_On;
            RM_D_On;
            RM_E_Off;
            RM_F_On;
            RM_G_On;
            break;
        default:
            RM_GPIO_ClearDigit();
    }
}

/******************************************************************************
	Input
	Output
	Description
        Turn off display
	Log
		01/15/18 Nickel Liang	First Draft
*******************************************************************************/
void RM_GPIO_ClearDigit(void) {
    RM_A_Off;
    RM_B_Off;
    RM_C_Off;
    RM_D_Off;
    RM_E_Off;
    RM_F_Off;
    RM_G_Off;
}

/******************************************************************************
	Input
	Output
	Description
        Test 7-seg display
	Log
		01/15/18 Nickel Liang	First Draft
*******************************************************************************/
void RM_GPIO_DigitTest(void) {
    int delay = 1000;
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(0);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(1);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(2);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(3);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(4);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(5);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(6);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(7);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(8);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
    RM_GPIO_DisplayDigit(9);
    HAL_Delay(delay);
    RM_GPIO_ClearDigit();
}
