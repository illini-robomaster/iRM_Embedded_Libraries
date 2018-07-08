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

#ifndef _BSP_TOF_H_
#define _BSP_TOF_H_

#include "vl53l0x_api.h"
#include "stm32f4xx_hal_gpio.h"

#define VL53L0X_DEFAULT_ADDR    0x52

#define MAX_BOOTUP_TIME         2

typedef struct {
    VL53L0X_Dev_t   *device;
    GPIO_TypeDef    *gport;
    uint16_t        gpin;
    uint32_t        ranging_time;
}   bsp_vl53l0x_t;

bsp_vl53l0x_t* bsp_vl53l0x_init(uint8_t addr, GPIO_TypeDef *gport, uint16_t gpin, uint32_t ranging_time);

uint8_t reset_vl53l0x(bsp_vl53l0x_t *tof);

uint16_t get_vl53l0x_dist_milli(bsp_vl53l0x_t *tof);

#endif
