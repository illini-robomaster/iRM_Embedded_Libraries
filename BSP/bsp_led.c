/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-14
 * @file    bsp_led.c
 * @brief   led board support package
 * @log     2018-04-14 nickelliang
 */

#include "bsp_led.h"

void led_red_on(void) {
    led_green_off();
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_RESET);
}

void led_red_off(void) {
    HAL_GPIO_WritePin(GPIOE, LED_RED_Pin, GPIO_PIN_SET);
}

void led_green_on(void) {
    led_red_off();
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_RESET);
}

void led_green_off(void) {
    HAL_GPIO_WritePin(GPIOF, LED_GREEN_Pin, GPIO_PIN_SET);
}

void led_on(void) {
    led_green_on();
    led_red_on();
}

void led_off(void) {
    led_green_off();
    led_red_off();
}
