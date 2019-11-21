#include "bsp_print.h"

#include "test_bsp_tof.h"
#include "cmsis_os.h"

void test_bsp_tof(void) {
    bsp_vl53l0x_t *tof1 = bsp_vl53l0x_init(0x80, GPIOC, GPIO_PIN_0, 30000);
    bsp_vl53l0x_t *tof2 = bsp_vl53l0x_init(0x70, GPIOC, GPIO_PIN_1, 30000);
    
    uint16_t dist1, dist2;
    while (1) {
        dist1 = get_vl53l0x_dist_milli(tof1);
        dist2 = get_vl53l0x_dist_milli(tof2);
        printf("dist1: %d dist2: %d\r\n", dist1, dist2);
        osDelay(50);
    }
}
