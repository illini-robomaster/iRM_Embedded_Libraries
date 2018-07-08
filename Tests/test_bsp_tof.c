#include "test_bsp_tof.h"
#include "bsp_print.h"
#include "cmsis_os.h"

void test_bsp_tof(void) {
    bsp_vl53l0x_t *tof1 = bsp_vl53l0x_init(0x80, GPIOC, GPIO_PIN_0, 30000);
    
    uint16_t dist1;
    while (1) {
        dist1 = get_vl53l0x_dist_milli(tof1);
        print("dist1: %d\r\n", dist1);
        osDelay(50);
    }
}
