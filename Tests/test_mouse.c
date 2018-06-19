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

#include "test_mouse.h"
#include "dbus.h"
#include "motor.h"
#include "pid.h"
#include "assert.h"

static int32_t clip_to_range(int32_t val, int32_t low, int32_t high) {
    if (val > high)
        return high;
    else if (val < low)
        return low;
    return val;
}

void init_yaw(motor_t *motor, pid_ctl_t *pid) {
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    can_motor_init(motor, 0x209, CAN1_ID, M6623);
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 5200, 6800, 2000, 500, 200, 9, 0.1, 14, 1600, 0);
#elif defined(ENGINEERING)
    can_motor_init(motor, 0x209, CAN1_ID, M6623);
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 3000, 6800, 1000000, 200, 200, 30, 0.02, 28, 3500, 0);
#elif defined(HERO)
    /* TODO */
    can_motor_init(motor, 0x209, CAN1_ID, M6623);
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 5200, 6800, 2000, 500, 200, 9, 0.1, 14, 1600, 0);
#endif
}

void init_pitch(motor_t *motor, pid_ctl_t *pid) {
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    can_motor_init(motor, 0x20A, CAN1_ID, M6623);
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 4800, 6200, 3000, 400, 100, 12, 0.2, 18, 3000, 0);
#elif defined(ENGINEERING)
    can_motor_init(motor, 0x205, CAN1_ID, M3510);
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 4000, 7900, 3000, 400, 200, 8, 0.05, 4, 3000, 0);
#elif defined(HERO)
    /* TODO */
    can_motor_init(motor, 0x20A, CAN1_ID, M6623);
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 1400, 2500, 3000, 400, 100, 12, 0.2, 18, 3000, 0);
#endif
}

static void init_shoot(motor_t *m_fy_left, motor_t *m_fy_right, motor_t *m_poke,
        pid_ctl_t *pid_fy_left, pid_ctl_t *pid_fy_right, pid_ctl_t *pid_poke) {
#if defined(INFANTRY1) || defined(INFANTRY2) || defined (INFANTRY3)
    can_motor_init(m_poke, 0x208, CAN1_ID, M3508);
    can_motor_init(m_fy_left, 0x205, CAN1_ID, M3508);
    can_motor_init(m_fy_right, 0x206, CAN1_ID, M3508);
    pid_init(pid_poke, POKE, m_poke, -2000, 0, 80000, 0, 0, 14, 2.3, 0, 10000, 0);
    pid_init(pid_fy_left, FLYWHEEL, m_fy_left, -9000, 0, 0, 0, 0, 22, 0, 0, 3000, 0);
    pid_init(pid_fy_right, FLYWHEEL, m_fy_right, 0, 9000, 0, 0, 0, 22, 0, 0, 3000, 0);
#elif defined(HERO)
    can_motor_init(m_poke, 0x208, CAN1_ID, M3508);
    can_motor_init(m_fy_left, 0x205, CAN1_ID, M3508);
    can_motor_init(m_fy_right, 0x206, CAN1_ID, M3508);
    pid_init(pid_poke, POKE, m_poke, -2000, 0, 80000, 0, 0, 14, 2.3, 0, 10000, 5, 0);
    pid_init(pid_fy_left, FLYWHEEL, m_fy_left, -9000, 0, 0, 0, 0, 22, 0, 0, 3000, 5, 0);
    pid_init(pid_fy_right, FLYWHEEL, m_fy_right, 0, 9000, 0, 0, 0, 22, 0, 0, 3000, 5, 0);
#endif
}

static void test_engineering_mouse() {
    dbus_t *rc = dbus_get_struct();
    motor_t     m_yaw, m_pitch;
    pid_ctl_t   pid_yaw, pid_pitch;

    init_yaw(&m_yaw, &pid_yaw);
    init_pitch(&m_pitch, &pid_pitch);

    int32_t yaw_ang = 4300, pitch_ang = 7300;
    while (1) {
        yaw_ang -= rc->mouse.x * 0.2;
        pitch_ang -= rc->mouse.y * 0.2;
        m_yaw.out = pid_calc(&pid_yaw, yaw_ang);
        m_pitch.out = pid_calc(&pid_pitch, pitch_ang);
        yaw_ang = clip_to_range(yaw_ang - rc->mouse.x * 0.2,
                pid_yaw.low_lim, pid_yaw.high_lim);
        pitch_ang = clip_to_range(pitch_ang - rc->mouse.y * 0.2,
                pid_pitch.low_lim, pid_pitch.high_lim);

        set_can_motor_output(&m_pitch, NULL, NULL, NULL);
        set_can_motor_output(&m_yaw, NULL, NULL, NULL);
        HAL_Delay(5);
    }
}

static void test_hero_mouse() {
    dbus_t *rc = dbus_get_struct();
    motor_t     m_pitch, m_fy_left, m_fy_right, m_poke;
    pid_ctl_t   pid_pitch, pid_fy_left, pid_fy_right, pid_poke;

    init_pitch(&m_pitch, &pid_pitch);
    init_shoot(&m_fy_left, &m_fy_right, &m_poke,
            &pid_fy_left, &pid_fy_right, &pid_poke);

    int32_t pitch_ang = 2000;
    int32_t flywheel_speed = 4000;
    int32_t poke_speed = 300;
    while (1) {
        if (rc->key.bit.Z)
            flywheel_speed = 4000;
        else if (rc->key.bit.X)
            flywheel_speed = 5000;
        else if (rc->key.bit.C)
            flywheel_speed = 6000;
        else if (rc->key.bit.V)
            flywheel_speed = 7000;
        else if (rc->key.bit.B)
            flywheel_speed = 8000;
        pitch_ang = clip_to_range(pitch_ang - rc->mouse.y * 0.2,
                pid_pitch.low_lim, pid_pitch.high_lim);
        m_pitch.out = pid_calc(&pid_pitch, pitch_ang);
        m_poke.out = m_fy_left.out = m_fy_right.out = 0;
        if (rc->key.bit.R) {
            m_fy_left.out = pid_calc(&pid_fy_left, -flywheel_speed);
            m_fy_right.out = pid_calc(&pid_fy_right, flywheel_speed);
            if (rc->mouse.l)
                m_poke.out = pid_calc(&pid_poke, -poke_speed);
        }
        else {
            m_fy_left.out = pid_calc(&pid_fy_left, 0);
            m_fy_right.out = pid_calc(&pid_fy_right, 0);
        }

        set_can_motor_output(NULL, &m_pitch, NULL, NULL);
        set_can_motor_output(&m_fy_left, &m_fy_right, NULL, &m_poke);
        HAL_Delay(5);
    }
}

static void test_infantry_mouse() {
    dbus_t *rc = dbus_get_struct();
    motor_t     m_pitch, m_fy_left, m_fy_right, m_poke;
    pid_ctl_t   pid_pitch, pid_fy_left, pid_fy_right, pid_poke;

    init_pitch(&m_pitch, &pid_pitch);
    init_shoot(&m_fy_left, &m_fy_right, &m_poke,
            &pid_fy_left, &pid_fy_right, &pid_poke);

    int32_t yaw_ang = 6000, pitch_ang = 5500;
    int32_t flywheel_speed = 4000;
    int32_t poke_speed = 300;
    while (1) {
        if (rc->key.bit.Z)
            flywheel_speed = 4000;
        else if (rc->key.bit.X)
            flywheel_speed = 5000;
        else if (rc->key.bit.C)
            flywheel_speed = 6000;
        else if (rc->key.bit.V)
            flywheel_speed = 7000;
        else if (rc->key.bit.B)
            flywheel_speed = 8000;
        pitch_ang = clip_to_range(pitch_ang - rc->mouse.y * 0.2,
                pid_pitch.low_lim, pid_pitch.high_lim);
        m_pitch.out = pid_calc(&pid_pitch, pitch_ang);
        m_poke.out = m_fy_left.out = m_fy_right.out = 0;
        if (rc->key.bit.R) {
            m_fy_left.out = pid_calc(&pid_fy_left, -flywheel_speed);
            m_fy_right.out = pid_calc(&pid_fy_right, flywheel_speed);
            if (rc->mouse.l)
                m_poke.out = pid_calc(&pid_poke, -poke_speed);
        }
        else {
            m_fy_left.out = pid_calc(&pid_fy_left, 0);
            m_fy_right.out = pid_calc(&pid_fy_right, 0);
        }

        set_can_motor_output(NULL, &m_pitch, NULL, NULL);
        set_can_motor_output(&m_fy_left, &m_fy_right, NULL, &m_poke);
        HAL_Delay(5);
    }
}

void test_mouse() {
#if defined(ENGINEERING)
    test_engineering_mouse();
#elif defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    test_infantry_mouse();
#elif defined(HERO)
    test_hero_mouse();
#endif
}
