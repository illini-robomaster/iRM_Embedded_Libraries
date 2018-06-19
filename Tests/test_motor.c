/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.  *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "test_motor.h"
#include "motor.h"
#include "bsp_print.h"

void test_motor() {
    // motor_feedback();
    // test_motor_3508(0);
    // test_motor_6623(0);
    // test_motor_2006(0);
    // test_motor_3510(0);
    test_motor_2305();
}

void motor_feedback(void) {
    motor_t *motor;
    motor = can_motor_init(NULL, 0x209, CAN1_ID, M6623);

    while (1) {
        get_motor_data(motor);
        print_motor_data(motor);
        HAL_Delay(20);
    }
}

void test_motor_3508(uint8_t rotate) {
    motor_t motor;
    size_t i;

    can_motor_init(&motor, 0x201, CAN1_ID, M3508);
    motor.out = 200;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_can_motor_output(&motor, NULL, NULL, NULL);

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

    can_motor_init(&motor, 0x20A, CAN1_ID, M6623);
    motor.out = 400;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_can_motor_output(&motor, NULL, NULL, NULL);

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}

void test_motor_2006(uint8_t rotate) {
    motor_t motor;
    size_t i;

    can_motor_init(&motor, 0x206, CAN1_ID, M2006);
    motor.out = 400;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_can_motor_output(NULL, &motor, NULL, NULL);

    for (i = 0; i < 1000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}

void test_motor_3510(uint8_t rotate) {
    motor_t motor;
    size_t i;

#ifdef ENGINEERING //engineering pitch
    can_motor_init(&motor, 0x205, CAN1_ID, M3510);
#else
    can_motor_init(&motor, 0x207, CAN1_ID, M3510);
#endif
    motor.out = 4000;

    if (rotate)
        for (i = 0; i < 1000; i++)
            set_can_motor_output(NULL, NULL, &motor, NULL);

    for (i = 0; i < 10000; i++) {
        get_motor_data(&motor);
        print_motor_data(&motor);
        HAL_Delay(20);
    }
}

void test_motor_2305(void) {
    pwm_t   pwm1, pwm2;
    motor_t motor1, motor2;

    pwm_init(&pwm1, &htim12, 1);
    pwm_init(&pwm2, &htim12, 2);
    pwm_motor_init(&motor1, M2305, &pwm1, 1000);
    pwm_motor_init(&motor2, M2305, &pwm2, 1000);

    osDelay(4000);

    while (1) {
        for (size_t i = 500; i < 1000; i += 0) {
            motor1.out = i;
            motor2.out = i;
            set_pwm_motor_output(&motor1);
            set_pwm_motor_output(&motor2);
            osDelay(1000);
        }
    }
}
