/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-12
 * @file    bsp_gpio.c
 * @brief   GPIO Board Support Package
 * @log     2018-04-13 nickelliang
 */

#include "bsp_gpio.h"

/* @todo Need to add support for upto 5 switches */
/**
 * This is a weak function, declared in stm32f4xx_hal_gpio.c
 *
 * @param  GPIO_Pin   A GPIO pin that can generate external interrupt signal
 * @author Nickel_Liang
 * @date   2018-05-27
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch (GPIO_Pin) {
        case SWITCH_1_Pin:
            /* @todo Add switch callback function here */
            break;
        case KEY_Pin:
            key_fsm();
            break;
        case SWITCH_2_Pin:
            /* @todo Add switch callback function here */
            break;
        case SD_EXTI_Pin:
            /* @todo Add SD callback function here */
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "Undefined GPIO callbacked.");
    }
}
