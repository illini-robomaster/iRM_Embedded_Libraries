/**
 * @author  Eric_Liang <hanghang177>
 * @date    2018-04-20
 * @file    test_servo.c
 * @brief   Servo lib test
 * #log     2018-04-20 hanghang177
 */

#include "test_servo.h"
#include "servo.h"

void test_servo() {
    servo_t servo;
    size_t i;

    servo_init(&servo, 5, 1, 1500);

    servo_setmax(&servo, 120);
    servo_setmin(&servo, 0);

    if (1) {
        for (i = 0; i <= 120; i+=10) {
            servo_write(&servo, i);
            HAL_Delay(1000);
        }
        servo_writeMicroseconds(&servo, 1500);
    }

}

void test_snail() {
    servo_t servo;
    size_t i;

    servo_init(&servo, 5, 1, 1000);

    if (1) {
        for (i = 1000; i < 2000; i++) {
          servo_writeMicroseconds(&servo, i);
          HAL_Delay(20);
        }
        servo_writeMicroseconds(&servo, 1000);
    }
}
