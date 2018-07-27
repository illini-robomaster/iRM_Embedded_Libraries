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

#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal_adc.h"
#include "adc.h"
#include "bsp_error_handler.h"
#include "bsp_config.h"

/* @note remember to add #include "stm32f4xx_hal_dma.h" to Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc.h */


typedef struct  adc_list_s {
    ADC_HandleTypeDef   *hadc;
    uint16_t            *data;
    uint16_t            buffer_size;
    uint8_t             channel_size;
    struct adc_list_s   *next;
}               adc_list_t;

typedef struct {
    adc_list_t          *shared_adc;
    uint32_t            channel;
    uint16_t            avg_filter_size;
}   adc_t;


/**
 * @brief enable generic ADC controller under DMA mode
 * @param hadc      ADC handle
 * @param pData     address of the first converted value
 * @param channel   total channel size for ADC1 (same as the maximum rank number marked
 *                  in ioc setting)
 * @return  1 for success, 0 for failure
 */
adc_t* adc_dma_enable(adc_t *my_adc, ADC_HandleTypeDef *hadc, uint8_t channel, uint16_t avg_filter_size);

/**
 * @brief get adc raw value given an adc type instance
 * @param my_adc adc type instance containing adc number and channel information
 * @return raw value in the range [0 4095]
 */
uint16_t adc_get_val(adc_t *my_adc);

/**
 * @brief get adc voltage value given an adc type instance
 * @param my_adc adc type instance containing adc number and channel information
 * @return raw value in the range [0 3.3]
 */
float adc_get_volt(adc_t *my_adc);

/**
 * @brief convert raw analog output to voltage value
 * @param raw_data raw analog output ranging from 0 ~ 4096
 * @return  converted voltage value
 */
float get_volt_from_raw_data(uint16_t raw_data);

#ifdef __cplusplus
}
#endif

#endif
