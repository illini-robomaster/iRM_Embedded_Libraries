#include "test_pid.h"
#include "motor.h"
#include "pid.h"
#include "bsp_print.h"
#include "stdlib.h"

#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x > y ? x : y

void test_pid() {
    // test_pid_3508();
    test_pid_6623();
}

void test_pid_3508() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;

    motor_init(&motor, 0x201, CAN1_ID, M3508);
    pid_init(&pid, CHASSIS_ROTATE, &motor, 0.9, 0.1, 8, 0, 1000, 0, 5);

    for (i = 0; i < 50000; i++) {
        get_motor_data(&motor);
        pid_calc(&pid, 400);
        set_motor_output(&motor, NULL, NULL, NULL);
        HAL_Delay(5);
    }
}

void test_pid_6623() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;

    motor_init(&motor, 0x20A, CAN1_ID, M6623);
    //we must send some nonzero data to initialize CAN
    for (i = 0; i < 100; i++) {
        get_motor_data(&motor);
        motor.out = 1;
        set_motor_output(&motor, NULL, NULL, NULL);
    }
    //kp = 5
    //ki = 0.03
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, 4.7, 0.03, 40, 4800, 6200, 200, 2);
    int target_val_1 = 5800;
    int target_val_2 = 5300;
    while (1) {
        motor.out = pid_calc(&pid, target_val_1);
        for (i = 0; i < 400; i++) {
            get_motor_data(&motor);
            int16_t err = get_angle_err(&motor, target_val_1);
            motor.out = pid_calc(&pid, target_val_1);
            set_motor_output(NULL, &motor, NULL, NULL);
            HAL_Delay(2);
        }
        motor.out = pid_calc(&pid, target_val_2);
        for (i = 0; i < 400; i++) {
            get_motor_data(&motor);
            int16_t err = get_angle_err(&motor, target_val_2);
            motor.out = pid_calc(&pid, target_val_2);
            set_motor_output(NULL, &motor, NULL, NULL);
            HAL_Delay(2);
        }
    }
}

void test_pid_2006() {
    motor_t motor;
    size_t i;

    motor_init(&motor, 0x202, CAN1_ID, M2006);
    motor.out = 400;

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}

void test_pid_3510() {
    motor_t motor;
    size_t i;

    motor_init(&motor, 0x205, CAN1_ID, M3510);
    motor.out = 4000;

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}
