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

#ifndef _BSP_POWER_H_
#define _BSP_POWER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_adc.h"
#include "bsp_config.h"

/**
 * @ingroup bsp
 * @defgroup bsp_power BSP Power
 * @{
 */

#define MAX_CURRENT_ERROR 3
#define MAX_VOLTAGE_ERROR 2

#define PM_OFFLINE  0
#define PM_ONLINE   1

/**
 * @brief initialize onboard power module
 * @param v_div     initial voltage divider (voltmeter)
 * @param a_p_v     initial ampere per voltage (amperemeter)
 * @param a_offset  ampere offset in adc voltage (in V)
 * @note    power voltage = adc voltage * voltage divider
 * @note    power current = (adc voltage + ampere offset) * ampere per voltage
 * @note v_div & a_offset can be calculated through calibration, however,
 *       an approximate value should be provided for offline detection
 *       to work properly
 */
void power_module_init(float v_div, float a_p_v, float a_offset);

/**
 * @brief power module offline detection by comparing power module adc input value
 *        with some true values
 * @param tru_volt  true voltage value (typically comes from referee)
 * @param tru_cur   true current value (typically comming from referee)
 * @return 1 if power module is online else 0
 * @note this offline check should be run BEFORE calibration
 */
uint8_t power_module_check(float tru_volt, float tru_cur);

/**
 * @brief get power module status (offline / online)
 * @return PM_ONLINE / PM_OFFLINE
 */
uint8_t power_module_stat(void);

/**
 * @brief calibrate voltmeter
 * @param volt measured voltage (potentially from referee system)
 */
void voltage_calibrate(float volt);

/**
 * @brief calibrate amperemeter
 * @param amp measured current (potentially from referee system)
 */
void current_calibrate(float amp);

/**
 * @brief calibrate both voltmeter and amperemeter
 * @param volt measured voltage (in Volts)
 * @param amp measured current (in Amperes)
 */
void power_module_calibrate(float volt, float amp);
    
/**
 * @brief get voltage value from power module
 * @return power voltage (in Volts)
 */
float get_volt(void);

/**
 * @brief get current value from power module
 * @return power current (in Amperes)
 */
float get_current(void);

/**
 * @brief get power value from power module
 * @return power value (in Jouls)
 */
float get_power(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
