#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "stm32f4xx_hal_adc.h"
#include "adc.h"
#include "bsp_error_handler.h"

#define BSP_ADC1_CHANNEL_SIZE 2
// #define BSP_ADC2_CHANNEL_SIZE 1
// #define BSP_ADC3_CHANNEL_SIZE 1

#define MAX_VOLT    3.3
#define MAX_ADC_OUT 4095

/**
 * @brief enable generic ADC controller under DMA mode
 * @param hadc      ADC handle
 * @param pData     address of the first converted value
 * @param channel   total channel size for ADC1 (same as the maximum rank number marked
 *                  in ioc setting)
 * @return  1 for success, 0 for failure 
 */
uint8_t adc_dma_enable(ADC_HandleTypeDef *hadc, uint16_t *pData, uint32_t channel);

/**
 * @brief enable ADC1 controller under DMA mode
 * @return 1 for suecess, 0 fo r failure
 */
uint8_t adc1_dma_enable(void);

/**
 * @brief read raw value from ADC1
 * @param channel which channel to be read (same as the rank number marked in ioc settings)
 * @return analog output ranging from 0 ~ 4095 (a read failure will return 0) 
 */
uint16_t adc1_get_val(uint8_t channel);

/**
 * @brief read voltage value from ADC1
 * @param channel which channel to be read (same as the rank number marked in ioc settings)
 * @return voltage value ranging from 0 ~ 3.3 (a read failure will return 0)
 */
float adc1_get_volt(uint8_t channel);

/**
 * @brief convert raw analog output to voltage value
 * @param raw_data raw analog output ranging from 0 ~ 4096
 * @return  converted voltage value 
 */
float get_volt_from_raw_data(uint16_t raw_data);

#endif
