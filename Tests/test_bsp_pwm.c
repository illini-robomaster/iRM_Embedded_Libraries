#include "test_bsp_pwm.h"
#include "cmsis_os.h"

void test_bsp_pwm() {
    pwm_t snail_left, snail_right;
    pwm_init(&snail_left, &htim12, 2);
    pwm_init(&snail_right, &htim12, 1);

    osDelay(4000);
    
    while (1) {
        pwm_set_pulse_width(&snail_left, 1280);
        pwm_set_pulse_width(&snail_right, 1280);
        osDelay(10000);
}
}
