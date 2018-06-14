#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

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

uint16_t adc_get_val(adc_t *my_adc);

float adc_get_volt(adc_t *my_adc);

/**
 * @brief convert raw analog output to voltage value
 * @param raw_data raw analog output ranging from 0 ~ 4096
 * @return  converted voltage value
 */
float get_volt_from_raw_data(uint16_t raw_data);

#endif
