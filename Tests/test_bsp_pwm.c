#include "test_bsp_pwm.h"
#include "cmsis_os.h"

void test_bsp_pwm() {
    pwm_t snail_2308;
    pwm_init(&snail_2308, &htim12, 2);

    osDelay(4000);
    
    while (1) {
        for (size_t i = 1100; i < 1230; i += 10) {
            pwm_set_pulse_width(&snail_2308, i);
            osDelay(1000);
        }
    }
}
