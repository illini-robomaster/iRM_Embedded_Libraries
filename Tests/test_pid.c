#include "test_pid.h"
#include "motor.h"
#include "poker.h"
#include "pid.h"
#include "bsp_print.h"
#include "stdlib.h"
#include "dbus.h"

#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x > y ? x : y

void test_pid() {
    // new_test_poke();
    // test_poke();
    // test_shoot();
    // test_chassis();
    test_pitch();
    // test_yaw();
}

void test_chassis() {
    motor_t m_fl, m_fr, m_rl, m_rr;
    pid_ctl_t pid1, pid2, pid3, pid4;
    dbus_t* rc = dbus_get_struct();
    int32_t speed = 2700;
    float kp = 10;
    float ki = 0.9;
    float kd = 0;
    int32_t int_lim = 200;
    size_t i;

    motor_init(&m_fl, 0x201, CAN1_ID, M3508);
    motor_init(&m_fr, 0x202, CAN1_ID, M3508);
    motor_init(&m_rl, 0x203, CAN1_ID, M3508);
    motor_init(&m_rr, 0x204, CAN1_ID, M3508);
    pid_init(&pid1, CHASSIS_ROTATE, &m_fl, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 5, 0);
    pid_init(&pid2, CHASSIS_ROTATE, &m_fr, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 5, 0);
    pid_init(&pid3, CHASSIS_ROTATE, &m_rl, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 5, 0);
    pid_init(&pid4, CHASSIS_ROTATE, &m_rr, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 5, 0);

    for (i = 0; i < 100; i++) {
        get_motor_data(&m_fl);
        m_fl.out = 1;
        set_motor_output(&m_fl, &m_fl, &m_fl, &m_fl);
    }
    while (1) {
        if (rc->key.bit.W) {
            m_fl.out = pid_calc(&pid1, speed);
            m_fr.out = pid_calc(&pid2, -speed);
            m_rl.out = pid_calc(&pid3, speed);
            m_rr.out = pid_calc(&pid4, -speed);
        }
        else if (rc->key.bit.S) {
            m_fl.out = pid_calc(&pid1, -speed);
            m_fr.out = pid_calc(&pid2, speed);
            m_rl.out = pid_calc(&pid3, -speed);
            m_rr.out = pid_calc(&pid4, speed);
        }
        else if (rc->key.bit.A) {
            m_fl.out = pid_calc(&pid1, -speed);
            m_fr.out = pid_calc(&pid2, -speed);
            m_rl.out = pid_calc(&pid3, speed);
            m_rr.out = pid_calc(&pid4, speed);
        }
        else if (rc->key.bit.D) {
            m_fl.out = pid_calc(&pid1, speed);
            m_fr.out = pid_calc(&pid2, speed);
            m_rl.out = pid_calc(&pid3, -speed);
            m_rr.out = pid_calc(&pid4, -speed);
        }
        else if (rc->key.bit.Q) {
            m_fl.out = pid_calc(&pid1, -speed);
            m_fr.out = pid_calc(&pid2, -speed);
            m_rl.out = pid_calc(&pid3, -speed);
            m_rr.out = pid_calc(&pid4, -speed);
        }
        else if (rc->key.bit.E) {
            m_fl.out = pid_calc(&pid1, speed);
            m_fr.out = pid_calc(&pid2, speed);
            m_rl.out = pid_calc(&pid3, speed);
            m_rr.out = pid_calc(&pid4, speed);
        }
        else {
            m_fl.out = pid_calc(&pid1, 0);
            m_fr.out = pid_calc(&pid2, 0);
            m_rl.out = pid_calc(&pid3, 0);
            m_rr.out = pid_calc(&pid4, 0);
        }
        set_motor_output(&m_fl, &m_fr, &m_rl, &m_rr);
        HAL_Delay(5);
    }
}

void test_pitch() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;

#ifdef ENGINEERING
    motor_init(&motor, 0x205, CAN1_ID, M3510);
#else
    motor_init(&motor, 0x20A, CAN1_ID, M6623);
#endif
    /*
     * p: 7.7
     * i: 0.2
     * d: 130
     */
#ifdef ENGINEERING
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, 1000, 5000, 0, 400, 200, 5, 0, 0, 3000, 5, 0);
#else
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, 4800, 6200, 0, 400, 200, 7.7, 0.2, 130, 3000, 5, 0);
#endif
    //we must send some nonzero data to initialize CAN
    for (i = 0; i < 100; i++) {
        get_motor_data(&motor);
        motor.out = 1;
        set_motor_output(NULL, &motor, NULL, NULL);
    }
    int target_val_1 = 6800;
    int target_val_2 = 5200;
    int target_val;
    while (1) {
        for (target_val = 1000; target_val < 5000; target_val += 200) {
            for (i = 0; i < 400; i++) {
                motor.out = pid_calc(&pid, target_val);
                set_motor_output(&motor, NULL, NULL, NULL);
                HAL_Delay(1);
            }
        }
    }
}

void test_yaw() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;

    motor_init(&motor, 0x209, CAN1_ID, M6623);
    //we must send some nonzero data to initialize CAN
    for (i = 0; i < 100; i++) {
        get_motor_data(&motor);
        motor.out = 1;
        set_motor_output(&motor, NULL, NULL, NULL);
    }
    pid_init(&pid, GIMBAL_MAN_SHOOT, &motor, 5200, 6800, 2000, 500, 200, 9, 0.1, 70, 1600, 5, 0);
    int target_val_1 = 6000;
    int target_val_2 = 5600;
    int target_val = 5600;
    while (1) {
        for (target_val = 5200; target_val < 6800; target_val += 200) {
            for (i = 0; i < 400; i++) {
                motor.out = pid_calc(&pid, target_val);
                set_motor_output(&motor, NULL, NULL, NULL);
                HAL_Delay(5);
            }
        }
    }
}

void test_camera() {
    motor_t motor;
    pid_ctl_t pid;
    size_t i;

    motor_init(&motor, 0x200, CAN1_ID, M3510);
    for (i = 0; i < 100; i++) {
        get_motor_data(&motor);
        motor.out = 1;
        set_motor_output(&motor, &motor, &motor, &motor);
    }
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
    pid_init(&pid_pitch, GIMBAL_MAN_SHOOT, &mt_pitch, 4800, 6200, 3000, 500, 200, 7.7, 0.2, 130, 3000, 5, 0);
    pid_init(&pid_left, FLYWHEEL, &mt_l, -4000, 0, 0, 0, 0, fw_kp, fw_ki, fw_kd, 3000, 5, 0);
    pid_init(&pid_right, FLYWHEEL, &mt_r, 0, 4000, 0, 0, 0, fw_kp, fw_ki, fw_kd, 3000, 5, 0);
    mt_l.out = mt_r.out = mt_pitch.out = 1;

    for (i = 0; i < 100; i++) {
        get_motor_data(&mt_pitch);
        get_motor_data(&mt_r);
        get_motor_data(&mt_l);
        set_motor_output(&mt_l, &mt_r, NULL, NULL);
        set_motor_output(NULL, &mt_pitch, NULL, NULL);
    }

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
    pid_init(&pid_poke, POKE, &mt_poke, -2000, 0, 80000, 0, 0, 18, 0.15, 0, 10000, 5, 0);
    pid_init(&pid_l, FLYWHEEL, &mt_l, -4000, 0, 0, 0, 0, 22, 0, 0, 3000, 5, 0);
    pid_init(&pid_r, FLYWHEEL, &mt_r, 0, 4000, 0, 0, 0, 22, 0, 0, 3000, 5, 0);
    mt_poke.out = 1;
    mt_l.out = 1;
    mt_r.out = 1;
    for (i = 0; i < 100; i++) {
        get_motor_data(&mt_poke);
        get_motor_data(&mt_l);
        get_motor_data(&mt_r);
        set_motor_output(&mt_l, &mt_r, NULL, &mt_poke);
    }

    while (1) {
        mt_l.out = pid_calc(&pid_l, -target_speed);
        mt_r.out = pid_calc(&pid_r, target_speed);
        mt_poke.out = pid_calc(&pid_poke, -300);
        set_motor_output(&mt_l, &mt_r, NULL, &mt_poke);
    }
}

void new_test_poke(void) {
    poker_ctl_t my_poker_st;
    motor_t poker_motor;

    motor_init(&poker_motor, 0x208, CAN1_ID, M3508);
    poker_init(&my_poker_st, 30, 0, &poker_motor);

    single_shot(&my_poker_st);
}
