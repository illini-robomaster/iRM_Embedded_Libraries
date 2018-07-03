/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-28
 * @file    bsp_oled.h
 * @brief   Board support package for RM 2018 OLED module
 * @log     2018-05-28 nickelliang
 */

#ifndef _BSP_OLED_H_
#define _BSP_OLED_H_

#include "stm32f4xx_hal.h"
#include "bsp_error_handler.h"
#include "bsp_print.h"
#include "bsp_config.h"
#include "bsp_adc.h"
#include "spi.h"
#include "math.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define OLED_CMD        0x00
#define OLED_DATA       0x01

#define OLED_CMD_SET()  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define OLED_CMD_CLR()  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)

#define OLED_RST_SET()  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)
#define OLED_RST_CLR()  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)

void oled_write_byte(uint8_t data, uint8_t cmd);

void oled_write_bytes(uint8_t *data, uint16_t length, uint8_t cmd);

void oled_display_init(void);

void oled_button_init(void);

uint16_t oled_button_get_val(void);

float oled_button_get_volt(void);

#endif
