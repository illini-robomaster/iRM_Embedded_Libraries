#include "bsp_adc.h"

uint16_t adc1_buff[BSP_ADC1_CHANNEL_SIZE];
// uint16_t adc2_buff[BSP_ADC2_CHANNEL_SIZE];
// uint16_t adc3_buff[BSP_ADC3_CHANNEL_SIZE];

uint8_t adc_dma_enable(ADC_HandleTypeDef *hadc, uint16_t *pData, uint32_t channel) {
    if (HAL_ADC_Start(hadc) != HAL_OK) {
        bsp_error_handler(__FUNCTION__, __LINE__, "HAL ADC Start failed!");
        return 0;
    }

    if (HAL_ADC_Start_DMA(hadc, (uint32_t*)pData, channel) != HAL_OK) {
        bsp_error_handler(__FUNCTION__, __LINE__, "HAL ADC Start DMA failed!");
        return 0;
    }

    return 1;
}

uint8_t adc1_dma_enable(void) {
    return adc_dma_enable(&hadc1, adc1_buff, BSP_ADC1_CHANNEL_SIZE);
}

uint8_t adc1_get_val(uint8_t channel) {
    if (channel >= BSP_ADC1_CHANNEL_SIZE) {
        bsp_error_handler(__FUNCTION__, __LINE__, "exceed maximum channel size for ADC1");
        return 0;
    }
    return adc1_buff[channel] & 0x0fff;
}

float adc1_get_volt(uint8_t channel) {
    return get_volt_from_raw_data(adc1_get_val(channel));
}

float get_volt_from_raw_data(uint16_t raw_data) {
    return raw_data * MAX_VOLT / MAX_ADC_OUT;
}
