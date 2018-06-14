#include "bsp_adc.h"
#include "FreeRTOS.h"

adc_list_t *adcs = NULL;

static void push_adc(adc_list_t *adc) {
    if (!adcs) {
        adcs = adc;
        return;
    }
    
    adc_list_t *tmp = adcs;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = adc;
}

static adc_list_t *get_adc_node(ADC_HandleTypeDef *hadc) {
    adc_list_t *tmp = adcs;
    while (tmp) {
        if (tmp->hadc == hadc)
            return tmp;
        tmp = tmp->next;
    }

    return NULL;
}

static adc_list_t* create_shared_adc_node(ADC_HandleTypeDef *hadc) {
    uint16_t channel_size = hadc->Init.NbrOfConversion;
    uint16_t buffer_size = channel_size * BSP_ADC_MAX_SAMPLE_SIZE;
    adc_list_t *shared_adc = pvPortMalloc(sizeof(adc_list_t));

    shared_adc->hadc            = hadc;
    shared_adc->channel_size    = channel_size;
    shared_adc->buffer_size     = buffer_size;
    shared_adc->data            = pvPortMalloc(buffer_size * sizeof(uint16_t));
    if (HAL_ADC_Start_DMA(hadc, (uint32_t*)(shared_adc->data), buffer_size) != HAL_OK) {
        vPortFree(shared_adc->data);
        vPortFree(shared_adc);
        bsp_error_handler(__FUNCTION__, __LINE__, "HAL ADC Start DMA failed!");
        return NULL;
    }
    push_adc(shared_adc);
    return shared_adc;
}

adc_t* adc_dma_enable(adc_t *my_adc, ADC_HandleTypeDef *hadc, 
        uint8_t channel, uint16_t avg_filter_size) {
    if (!my_adc)
        my_adc = pvPortMalloc(sizeof(adc_t));
    my_adc->channel                     = channel;
    my_adc->avg_filter_size             = avg_filter_size;

    adc_list_t *shared_adc = get_adc_node(hadc);
    if (!shared_adc)
        shared_adc = create_shared_adc_node(hadc);
    if (!shared_adc) {
        bsp_error_handler(__FUNCTION__, __LINE__, "shared adc node create failed");
        return NULL;
    }

    my_adc->shared_adc = shared_adc;
    return my_adc;
}

uint16_t adc_get_val(adc_t *my_adc) {
    uint32_t result = 0;
    uint32_t sample_cnt = 0;
    uint8_t channel_size = my_adc->shared_adc->channel_size;
    int16_t   i = my_adc->shared_adc->hadc->DMA_Handle->Instance->NDTR;

    i = (i / channel_size) * channel_size + my_adc->channel;
    i = i % my_adc->shared_adc->buffer_size;

    while (sample_cnt < my_adc->avg_filter_size) {
        result += my_adc->shared_adc->data[i] & 0x0fff;
        i -= channel_size;
        if (i < 0)
            i += my_adc->shared_adc->buffer_size;
        sample_cnt++;
    }
    return result / my_adc->avg_filter_size;
}

float adc_get_volt(adc_t *my_adc) {
    uint16_t raw_value = adc_get_val(my_adc);
    return get_volt_from_raw_data(raw_value);
}

float get_volt_from_raw_data(uint16_t raw_data) {
    return raw_data * BSP_ADC_MAX_VOLT / BSP_ADC_MAX_ADC_OUT;
}

