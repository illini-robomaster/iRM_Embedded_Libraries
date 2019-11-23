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

#include "test_chassis.h"
#include "test_pid.h"
#include "motor.h"
#include "pid.h"
#include "stdlib.h"
#include "dbus.h"
#include "test_mouse.h"

#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x > y ? x : y

static void init_shoot(motor_t *m_fy_left, motor_t *m_fy_right, motor_t *m_poke,
        pid_ctl_t *pid_fy_left, pid_ctl_t *pid_fy_right, pid_ctl_t *pid_poke) {
    size_t i;
    can_motor_init(m_poke, 0x208, CAN1_ID, M3508);
    can_motor_init(m_fy_left, 0x205, CAN1_ID, M3508);
    can_motor_init(m_fy_right, 0x206, CAN1_ID, M3508);
    pid_init(pid_poke, POKE, m_poke, -2000, 0, 80000, 0, 0, 14, 2, 0, 10000, 0);
    pid_init(pid_fy_left, FLYWHEEL, m_fy_left, -4000, 0, 0, 0, 0, 22, 0, 0, 3000, 0);
    pid_init(pid_fy_right, FLYWHEEL, m_fy_right, 0, 4000, 0, 0, 0, 22, 0, 0, 3000, 0);
    m_fy_left->out = m_fy_right->out = 1;

    for (i = 0; i < 100; i++) {
        get_motor_data(m_fy_right);
        get_motor_data(m_fy_left);
        get_motor_data(m_poke);
        set_can_motor_output(m_fy_left, m_fy_right, NULL, m_poke);
    }
}

void demo_run_chassis() {
    motor_t m_fl, m_fr, m_rl, m_rr, m_pitch, m_yaw;
    pid_ctl_t pid_fl, pid_fr, pid_rl, pid_rr, pid_pitch, pid_yaw;
    dbus_t* rc = dbus_get_struct();
    int32_t yaw_ang, pitch_ang;
    int32_t speed = 1300;
    float kp = 13;
    float ki = 0.1;
    float kd = 0;
    int32_t int_lim = 3000;
    size_t i;

    init_yaw(&m_yaw, &pid_yaw);
    init_pitch(&m_pitch, &pid_pitch);
#ifdef ENGINEERING
    yaw_ang = 4300;
    pitch_ang = 7300;
#elif defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    yaw_ang = 6000;
    pitch_ang = 5500;
#endif
    can_motor_init(&m_fl, 0x201, CAN1_ID, M3508);
    can_motor_init(&m_fr, 0x202, CAN1_ID, M3508);
    can_motor_init(&m_rl, 0x203, CAN1_ID, M3508);
    can_motor_init(&m_rr, 0x204, CAN1_ID, M3508);
    pid_init(&pid_fl, CHASSIS_ROTATE, &m_fl, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 0);
    pid_init(&pid_fr, CHASSIS_ROTATE, &m_fr, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 0);
    pid_init(&pid_rl, CHASSIS_ROTATE, &m_rl, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 0);
    pid_init(&pid_rr, CHASSIS_ROTATE, &m_rr, -3000, 3000, int_lim, 0, 0, kp, ki, kd, 0, 0);

    for (i = 0; i < 100; i++) {
        get_motor_data(&m_fl);
        m_fl.out = 1;
        set_can_motor_output(&m_fl, &m_fl, &m_fl, &m_fl);
    }
    while (1) {
        if (rc->key.bit.W) {
            m_fl.out = pid_calc(&pid_fl, speed);
            m_fr.out = pid_calc(&pid_fr, -speed);
            m_rl.out = pid_calc(&pid_rl, speed);
            m_rr.out = pid_calc(&pid_rr, -speed);
        }
        else if (rc->key.bit.S) {
            m_fl.out = pid_calc(&pid_fl, -speed);
            m_fr.out = pid_calc(&pid_fr, speed);
            m_rl.out = pid_calc(&pid_rl, -speed);
            m_rr.out = pid_calc(&pid_rr, speed);
        }
        else if (rc->key.bit.A) {
            m_fl.out = pid_calc(&pid_fl, -speed);
            m_fr.out = pid_calc(&pid_fr, -speed);
            m_rl.out = pid_calc(&pid_rl, speed);
            m_rr.out = pid_calc(&pid_rr, speed);
        }
        else if (rc->key.bit.D) {
            m_fl.out = pid_calc(&pid_fl, speed);
            m_fr.out = pid_calc(&pid_fr, speed);
            m_rl.out = pid_calc(&pid_rl, -speed);
            m_rr.out = pid_calc(&pid_rr, -speed);
        }
        else if (rc->key.bit.Q) {
            m_fl.out = pid_calc(&pid_fl, -speed);
            m_fr.out = pid_calc(&pid_fr, -speed);
            m_rl.out = pid_calc(&pid_rl, -speed);
            m_rr.out = pid_calc(&pid_rr, -speed);
        }
        else if (rc->key.bit.E) {
            m_fl.out = pid_calc(&pid_fl, speed);
            m_fr.out = pid_calc(&pid_fr, speed);
            m_rl.out = pid_calc(&pid_rl, speed);
            m_rr.out = pid_calc(&pid_rr, speed);
        }
        else {
            m_fl.out = pid_calc(&pid_fl, 0);
            m_fr.out = pid_calc(&pid_fr, 0);
            m_rl.out = pid_calc(&pid_rl, 0);
            m_rr.out = pid_calc(&pid_rr, 0);
        }
        m_yaw.out = pid_calc(&pid_yaw, yaw_ang);
        m_pitch.out = pid_calc(&pid_pitch, pitch_ang);
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
        set_can_motor_output(&m_fl, &m_fr, &m_rl, &m_rr);
        set_can_motor_output(&m_yaw, &m_pitch, NULL, NULL);
#elif defined(ENGINEERING)
        set_can_motor_output(&m_yaw, NULL, NULL, NULL);
        set_can_motor_output(&m_pitch, NULL, NULL, NULL);
#else
        set_can_motor_output(&m_fl, &m_fr, &m_rl, &m_rr);
#endif
        HAL_Delay(5);
    }
}

void test_chassis() {
    demo_run_chassis();
}
