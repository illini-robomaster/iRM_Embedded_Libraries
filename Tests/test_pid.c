/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "test_pid.h"
#include "motor.h"
#include "pid.h"
#include "bsp_print.h"
#include "stdlib.h"
#include "dbus.h"
#include "utils.h"

void test_pid() {
    // new_test_poke();
    // test_poke();
    // test_shoot();
    // test_chassis();
    // test_pitch();
    // test_yaw();
    // test_2006();
}

void test_power_speed(void) {
    motor_t *chassis_mt[4];
    pid_ctl_t *chassis[4];
    pid_ctl_t *power_pid;
    uint32_t can_ids[4] = {0x201, 0x202, 0x203, 0x204};
    for (uint8_t i = 0; i < 4; ++i) {
        chassis_mt[i] = motor_init(NULL, can_ids[i], CAN1_ID, M3508);
        chassis[i] = pid_init(NULL, CHASSIS_ROTATE, chassis_mt[i], -5000, 5000, 200,
                    0, 0, 10, 0.9, 0, 0, 0);
    }
    power_pid = pid_init(NULL, POWER_CTL, chassis_mt[0], 0, 0, 0, 0, 0, 0.1, 0, 0, 0, 0);
    int32_t target_speed = 0;
    int32_t delta_speed = 0;
    dbus_t *rc = dbus_get_struct();
    uint32_t os_time = osKernelSysTick();
    while(1) {
        if (rc->swl == RC_SWITCH_UP) {
            delta_speed = pid_calc(power_pid, 8);
            target_speed += delta_speed;
        } else {
            target_speed = 0;
        }
        for (uint8_t i = 0; i < 4; i++) {
            chassis[i]->motor->out = pid_calc(chassis[i], target_speed);
        }
        set_motor_output(chassis_mt[0], chassis_mt[1], chassis_mt[2], chassis_mt[3]);
        osDelayUntil(&os_time, 20);
    }
}

void test_pitch() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;
    int32_t low_lim, high_lim;

#ifdef ENGINEERING
    low_lim = 5000;
    high_lim = 7700;
    motor_init(&motor, 0x205, CAN1_ID, M3510);
    /* TODO: needs to be re-tuned with camera load on */
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, low_lim, high_lim, 0, 0, 0, 4, 0, 8, 8000, 0);
#elif defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    low_lim = 5200;
    high_lim = 6800;
    motor_init(&motor, 0x20A, CAN1_ID, M6623);
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, low_lim, high_lim, 0, 0, 800, 4, 0.03, 15, 1800, 0);
#endif

    int target_val_1 = 6800;
    int target_val_2 = 5200;
    int target_val;
    uint32_t pid_pitch_time = osKernelSysTick();
    while (1) {
        for (target_val = low_lim; target_val < high_lim; target_val += 400) {
            for (i = 0; i < 100; i++) {
                motor.out = pid_calc(&pid, target_val);
#ifdef ENGINEERING
                set_motor_output(&motor, NULL, NULL, NULL);
#else
                set_motor_output(NULL, &motor, NULL, NULL);
#endif
                osDelayUntil(&pid_pitch_time, 20);
            }
        }
    }
}

void test_yaw() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;
    int target_val;

#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    motor_init(&motor, 0x209, CAN1_ID, M6623);
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, 5200, 6800, 0, 0, 0, 20, 0, 80, 4500, 0);
#elif defined(ENGINEERING)
    motor_init(&motor, 0x209, CAN1_ID, M6623);
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, 0, 0, 0, 0, 600, 7, 0, 32, 3500, 0);
#endif
    uint32_t pid_yaw_time = osKernelSysTick();
    while (1) {
        for (target_val = 400; target_val < 6800; target_val += 0) {
            for (i = 0; i < 100; i++) {
                motor.out = pid_calc(&pid, target_val);
                set_motor_output(&motor, NULL, NULL, NULL);
                osDelayUntil(&pid_yaw_time, 20);
            }
        }
    }
}

void test_camera() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;

    motor_init(&motor, 0x200, CAN1_ID, M3510);
}

void test_shoot() {
    motor_t mt_l, mt_r, mt_pitch;
    pid_ctl_t pid_left, pid_right, pid_pitch;
    size_t i;
    float fw_kp = 22;
    float fw_ki = 0;
    float fw_kd = 0;

    motor_init(&mt_pitch, 0x20A, CAN1_ID, M6623);
    motor_init(&mt_l, 0x205, CAN1_ID, M3508);
    motor_init(&mt_r, 0x206, CAN1_ID, M3508);
    /* TODO: Warning!! All PID requires re-tuning */
    pid_init(&pid_pitch, GIMBAL_MAN_SHOOT, &mt_pitch, 4800, 6200, 3000, 500, 200, 7.7, 0.2, 130, 3000, 0);
    pid_init(&pid_left, FLYWHEEL, &mt_l, -4000, 0, 0, 0, 0, fw_kp, fw_ki, fw_kd, 3000, 0);
    pid_init(&pid_right, FLYWHEEL, &mt_r, 0, 4000, 0, 0, 0, fw_kp, fw_ki, fw_kd, 3000, 0);
    mt_l.out = mt_r.out = mt_pitch.out = 1;

    int32_t target_speed = 2000;
    while (1) {
        mt_pitch.out = pid_calc(&pid_pitch, 5500);
        mt_l.out = pid_calc(&pid_left, -target_speed);
        mt_r.out = pid_calc(&pid_right, target_speed);
        set_motor_output(NULL, &mt_pitch, NULL, NULL);
        set_motor_output(&mt_l, &mt_r, NULL, NULL);
    }
}

void test_poke() {
    size_t i;
    motor_t mt_poke, mt_l, mt_r;
    pid_ctl_t pid_poke, pid_l, pid_r;
    int32_t target_speed = 1000;

    motor_init(&mt_poke, 0x208, CAN1_ID, M3508);
    motor_init(&mt_l, 0x205, CAN1_ID, M3508);
    motor_init(&mt_r, 0x206, CAN1_ID, M3508);
    /* TODO: Warning!! All PID requires re-tuning */
    pid_init(&pid_poke, POKE, &mt_poke, -2000, 0, 80000, 0, 0, 18, 0.15, 0, 10000, 0);
    pid_init(&pid_l, FLYWHEEL, &mt_l, -4000, 0, 0, 0, 0, 22, 0, 0, 3000, 0);
    pid_init(&pid_r, FLYWHEEL, &mt_r, 0, 4000, 0, 0, 0, 22, 0, 0, 3000, 0);
    mt_poke.out = 1;
    mt_l.out = 1;
    mt_r.out = 1;

    while (1) {
        mt_l.out = pid_calc(&pid_l, -target_speed);
        mt_r.out = pid_calc(&pid_r, target_speed);
        mt_poke.out = pid_calc(&pid_poke, -300);
        set_motor_output(&mt_l, &mt_r, NULL, &mt_poke);
    }
}

void test_2006() {
    motor_t     mt_2006;
    pid_ctl_t   pid_2006;

    motor_init(&mt_2006, 0x201, CAN1_ID, M2006);
    pid_init(&pid_2006, POKE, &mt_2006, -2000, 2000, 0, 0, 0, 3, 0.5, 0, 5000, 0);

    int32_t target_speed = 300;
    uint32_t mt_2006_wake_time = osKernelSysTick();

    while (1) {
        mt_2006.out = pid_calc(&pid_2006, target_speed);
        set_motor_output(&mt_2006, NULL, NULL, NULL);
        osDelayUntil(&mt_2006_wake_time, 20);
    }
}
