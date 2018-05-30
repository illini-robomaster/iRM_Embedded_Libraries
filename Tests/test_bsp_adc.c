#include "test_bsp_adc.h"
#include "cmsis_os.h"

void test_bsp_adc(void) {
    adc1_dma_enable();
    while (1) {
        uint16_t chan0 = adc1_get_val(0);
        uint16_t chan1 = adc1_get_val(1);
        print("channel 0: %d channel 1: %d\r\n", chan0, chan1);
        osDelay(300);
    }
}
