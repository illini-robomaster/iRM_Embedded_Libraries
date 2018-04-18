#include "test_motor.h"
#include "motor.h"

void test_motor() {
    test_motor_3508();
}

void test_motor_3508() {
    motor_t motor;
    size_t i;

    motor_init(&motor, 0x201, CAN1_ID, M3508);
    for (i = 0; i < 100000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(10);
    }
}
