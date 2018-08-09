/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

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

/* TODO: missing documentation */

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
