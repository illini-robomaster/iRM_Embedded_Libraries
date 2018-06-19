#include "test_bsp_pwm.h"
#include "cmsis_os.h"

void test_bsp_pwm() {
    pwm_t snail_left, snail_right;
    pwm_init(&snail_left, &htim12, 2);
    pwm_init(&snail_right, &htim12, 1);

    osDelay(4000);
    
    while (1) {
        for (size_t i = 1100; i < 1300; i += 50) {
            pwm_set_pulse_width(&snail_left, i);
            pwm_set_pulse_width(&snail_right, i);
            osDelay(10000);
        }
    }
}
