#ifndef _BSP_POWER_H_
#define _BSP_POWER_H_

#include "bsp_adc.h"
#include "bsp_config.h"

#define MAX_CURRENT_ERROR 3
#define MAX_VOLTAGE_ERROR 2

/**
 * @brief initialize onboard power module
 * @param v_div     initial voltage divider (voltmeter)
 * @param a_p_v     initial ampere per voltage (amperemeter)
 * @param a_offset  ampere offset in adc voltage (in V)
 * @note    power voltage = adc voltage * voltage divider
 * @note    power current = (adc voltage + ampere offset) * ampere per voltage
 */
void power_module_init(float v_div, float a_p_v, float a_offset);

/**
 * @brief power module offline detection by comparing power module adc input value
 *        with some true values
 * @param tru_volt  true voltage value (typically comes from referee)
 * @param tru_cur   true current value (typically comming from referee)
 * @return 1 if power module is online else 0
 */
uint8_t power_module_online(float tru_volt, float tru_cur);

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
 * @param volt measured voltage (in V)
 * @param amp measured current (in A)
 */
void power_module_calibrate(float volt, float amp);
    
/**
 * @brief get voltage value from power module
 * @return power voltage (in V)
 */
float get_volt(void);

/**
 * @brief get current calue from power module
 * @return power current (in A)
 */
float get_current(void);

#endif
