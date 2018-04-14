/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-14
 * @file    bsp_led.c
 * @brief   LED board support package
 * @log     2018-04-14 nickelliang
 */

#include "bsp_led.h"

void LED_red_on(void) {
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_RESET);
}

void LED_red_off(void) {
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_SET);
}

void LED_red_toggle(void) {
    HAL_GPIO_TogglePin(GPIOE, LED_RED_Pin);
}

void LED_green_on(void) {
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_RESET);
}

void LED_green_off(void) {
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_SET);
}

void LED_green_toggle(void) {
    HAL_GPIO_TogglePin(GPIOF, LED_GREEN_Pin);
}

void LED_on(void) {
    LED_green_on();
    LED_red_on();
}

void LED_off(void) {
    LED_green_off();
    LED_red_off();
}

void LED_toggle(void) {
    LED_green_toggle();
    LED_red_toggle();
}
