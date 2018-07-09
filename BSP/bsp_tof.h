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

/**
 * @struct  bsp_vl53l0x_t
 * @brief   store vl53l0x tof ranging sensor data
 * @var device  device information with official driver type
 * @var gport           gpio group number
 * @var gpin            gpio pin number
 * @var ranging_time    ranging time budget in microseconds
 */
typedef struct {
    VL53L0X_Dev_t   *device;
    GPIO_TypeDef    *gport;
    uint16_t        gpin;
    uint32_t        ranging_time;
}   bsp_vl53l0x_t;

/**
 * @brief initialize vl53l0x device
 * @param addr          device i2c address (to be set)
 * @param gport         gpio group number for reset pin
 * @param gpin          gpio pin number for reset pin
 * @param ranging_time  ranging time budget in microseconds
 * @return allocated and initialized bsp_vl53l0x_t instance
 */
bsp_vl53l0x_t* bsp_vl53l0x_init(uint8_t addr, GPIO_TypeDef *gport, uint16_t gpin, uint32_t ranging_time);

/**
 * @brief in case of any error, reset the tof sensor through
 *        gpio reset pin
 * @param tof a bsp_vl53l0x_t type that holds all sensor information
 * @return 1 for success and 0 for failure
 */
uint8_t reset_vl53l0x(bsp_vl53l0x_t *tof);

/**
 * @brief read sensor ranging distance measurement in millimeter
 * @param tof a bsp_vl53l0x_t type that holds all sensor information
 * @return measured distance in millimeter
 */
uint16_t get_vl53l0x_dist_milli(bsp_vl53l0x_t *tof);

#endif
