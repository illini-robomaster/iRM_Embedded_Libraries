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

#include "bsp_power.h"
#include <math.h>

float v_divider;
float amp_per_volt;
float amp_offset;

adc_t *volt_adc;
adc_t *amp_adc;

uint8_t pm_stat = PM_OFFLINE;

void power_module_init(float v_div, float a_p_v, float a_offset) {
    volt_adc = adc_dma_enable(NULL, &BSP_POWER_ADC,
            BSP_POWER_V_CHANNEL, BSP_POWER_AVG_FILTER_SIZE);
    amp_adc = adc_dma_enable(NULL, &BSP_POWER_ADC,
            BSP_POWER_A_CHANNEL, BSP_POWER_AVG_FILTER_SIZE);
    v_divider       = v_div;
    amp_per_volt    = a_p_v;
    amp_offset      = a_offset;
}

uint8_t power_module_check(float tru_volt, float tru_cur) {
    if (fabs(tru_volt - get_volt()) <= MAX_VOLTAGE_ERROR &&
            fabs(tru_cur - get_current()) <= MAX_CURRENT_ERROR)
        pm_stat = PM_ONLINE;
    else
        pm_stat = PM_OFFLINE;

    return pm_stat;
}

uint8_t power_module_stat(void) {
    return pm_stat;
}

void voltage_calibrate(float volt) {
    float analog_volt = adc_get_volt(volt_adc);

    v_divider = volt / analog_volt;
}

void current_calibrate(float amp) {
    float true_analog_volt = adc_get_volt(amp_adc);
    float theoretical_analog_volt = amp / amp_per_volt;
    
    amp_offset = theoretical_analog_volt - true_analog_volt;
}

void power_module_calibrate(float volt, float amp) {
    voltage_calibrate(volt);
    current_calibrate(amp);
}

float get_volt(void) {
    float adc_volt = adc_get_volt(volt_adc);

    return adc_volt * v_divider;
}

float get_current(void) {
    float adc_volt = adc_get_volt(amp_adc);

    return (adc_volt + amp_offset) * amp_per_volt;
}

float get_power(void) {
    return get_volt() * get_current();
}
