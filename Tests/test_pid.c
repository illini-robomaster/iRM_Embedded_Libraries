#include "test_pid.h"
#include "motor.h"
#include "pid.h"
#include "bsp_print.h"
#include "stdlib.h"

#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x > y ? x : y

void test_pid() {
    // test_chassis();
    test_pitch();
}

void test_chassis() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;

    motor_init(&motor, 0x201, CAN1_ID, M3508);
    pid_init(&pid, CHASSIS_ROTATE, &motor, 0, 1000, 0, 0, 0, 0.9, 0.1, 8, 0, 5);

    for (i = 0; i < 50000; i++) {
        get_motor_data(&motor);
        pid_calc(&pid, 400);
        set_motor_output(&motor, NULL, NULL, NULL);
        HAL_Delay(5);
    }
}

void test_pitch() {
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
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, 4800, 6200, 0, 400, 200, 7.7, 0.2, 130, 3000, 5);
    int target_val_1 = 6800;
    int target_val_2 = 5200;
    int target_val;
    while (1) {
        for (target_val = 5200; target_val < 6200; target_val += 100) {
            motor.out = pid_calc(&pid, target_val);
            for (i = 0; i < 400; i++) {
                get_motor_data(&motor);
                int16_t err = get_angle_err(&motor, target_val);
                motor.out = pid_calc(&pid, target_val);
                set_motor_output(NULL, &motor, NULL, NULL);
                HAL_Delay(5);
            }
        }
    }
}
