/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-15
 * @file    bsp_laser.c
 * @brief   Board support package for laser
 * @log     2018-04-15 nickelliang
 */

#include "bsp_laser.h"

void laser_on(void) {
    HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET);
}

void laser_off(void) {
    HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_RESET);
}
