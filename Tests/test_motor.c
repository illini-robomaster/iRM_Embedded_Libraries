#include "test_motor.h"
#include "motor.h"
#include "bsp_print.h"

void test_motor() {
    // test_motor_3508(0);
    // test_motor_6623(1);
    // test_motor_2006(0);
    test_motor_3510(0);
}

void test_motor_3508(uint8_t rotate) {
    motor_t motor;
    size_t i;

    motor_init(&motor, 0x201, CAN1_ID, M3508);
    motor.out = 200;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_motor_output(&motor, NULL, NULL, NULL);

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        print("Target: %d Error: %d\n", 400, get_angle_err(&motor, 400));
        HAL_Delay(20);
    }
}

void test_motor_6623(uint8_t rotate) {
    motor_t motor;
    size_t i;

    motor_init(&motor, 0x209, CAN1_ID, M6623);
    motor.out = 400;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_motor_output(&motor, NULL, NULL, NULL);

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}

void test_motor_2006(uint8_t rotate) {
    motor_t motor;
    size_t i;

    motor_init(&motor, 0x202, CAN1_ID, M2006);
    motor.out = 400;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_motor_output(NULL, &motor, NULL, NULL);

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}

void test_motor_3510(uint8_t rotate) {
    motor_t motor;
    size_t i;

    motor_init(&motor, 0x207, CAN1_ID, M3510);
    motor.out = 4000;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_motor_output(NULL, NULL, &motor, NULL);

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}
