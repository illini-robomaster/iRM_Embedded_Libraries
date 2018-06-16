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

#include "chassis.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>

static pid_ctl_t chassis_rotate;
const static int16_t evasive_tar[2] = {EVASIVE_LEFTMOST_YAW, EVASIVE_RIGHTMOST_YAW};
static uint8_t evasive_tar_cnt = 0;

/**
 * backward:    -1
 * disabled:    0
 * forward:     1
 */
int8_t direction = 1;

static void normalize_to_range(float *vx, float *vy) {
    float norm = 1;
    if (*vx != 0 || *vy != 0)
        norm = sqrt(*vx * *vx + *vy * *vy);
    if (norm > 1) {
        *vx /= norm;
        *vy /= norm;
    }
}

void chassis_init(pid_ctl_t *my_chassis[4]){
    motor_t *m_fl, *m_fr, *m_rl, *m_rr;
    pid_ctl_t *pid_fl, *pid_fr, *pid_rl, *pid_rr;
    m_fl = m_fr = m_rl = m_rr = NULL;
    pid_fl = pid_fr = pid_rl = pid_rr = NULL;

    m_fl = motor_init(NULL, FL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_fr = motor_init(NULL, FR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rl = motor_init(NULL, RL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rr = motor_init(NULL, RR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);

    pid_fl = pid_init(NULL, CHASSIS_ROTATE, m_fl, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, 0);
    pid_fr = pid_init(NULL, CHASSIS_ROTATE, m_fr, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, 0);
    pid_rl = pid_init(NULL, CHASSIS_ROTATE, m_rl, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, 0);
    pid_rr = pid_init(NULL, CHASSIS_ROTATE, m_rr, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, 0);

    my_chassis[CHASSIS_FL] = pid_fl;
    my_chassis[CHASSIS_FR] = pid_fr;
    my_chassis[CHASSIS_RL] = pid_rl;
    my_chassis[CHASSIS_RR] = pid_rr;

    pid_init(&chassis_rotate, MANUAL_ERR_INPUT, m_fl, 0, 0,
                0, 0, 0, ROTATE_KP, 0, 0, MAX_TURN_SPEED, YAW_DEADBAND);
}

void calc_keyboard_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle) {
    // counterclockwise: positive
    yaw_angle = -yaw_angle + Q_PI;
    float v_y = 0;
    float v_x = 0;
    if (rc->key.bit.W) v_y += 1;
    if (rc->key.bit.S) v_y -= 1;
    if (rc->key.bit.A) v_x -= 1;
    if (rc->key.bit.D) v_x += 1;
    // rotation; change of basis matrix.
    float target_x = (v_x * cos(yaw_angle) + v_y * sin(yaw_angle)) * MAX_LINEAR_SPEED;
    float target_y = (-v_x * sin(yaw_angle) + v_y * cos(yaw_angle)) * MAX_LINEAR_SPEED;
    my_chassis[CHASSIS_FL]->motor->target = direction * target_x;
    my_chassis[CHASSIS_RR]->motor->target = direction * (-target_x); // velocity is the same. It's just these two motors are installed in opposite direction.
    my_chassis[CHASSIS_RL]->motor->target = direction * target_y;
    my_chassis[CHASSIS_FR]->motor->target = direction * (-target_y);
}

void calc_remote_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle) {
    yaw_angle = -yaw_angle + Q_PI;
    float v_y = rc->ch1 * 1.0 / 660;
    float v_x = rc->ch0 * 1.0 / 660;
    // rotation; change of basis matrix.
    float target_x = (v_x * cos(yaw_angle) + v_y * sin(yaw_angle)) * MAX_LINEAR_SPEED;
    float target_y = (-v_x * sin(yaw_angle) + v_y * cos(yaw_angle)) * MAX_LINEAR_SPEED;
    my_chassis[CHASSIS_FL]->motor->target = direction * target_x;
    my_chassis[CHASSIS_RR]->motor->target = direction * (-target_x); // velocity is the same. It's just these two motors are installed in opposite direction.
    my_chassis[CHASSIS_RL]->motor->target = direction * target_y;
    my_chassis[CHASSIS_FR]->motor->target = direction * (-target_y);
}

void evasive_move(pid_ctl_t *my_chassis[4], int16_t cur_yaw_feedback, motor_t *yaw_motor) {
    if(abs(cur_yaw_feedback - EVASIVE_LEFTMOST_YAW) < EVASIVE_DEADBAND) {
        evasive_tar_cnt = 1;
    }
    if(abs(cur_yaw_feedback - EVASIVE_RIGHTMOST_YAW) < EVASIVE_DEADBAND) {
        evasive_tar_cnt = 0;
    }
    adjust_chassis_gimbal_pos(my_chassis, evasive_tar[evasive_tar_cnt], yaw_motor);
}

void adjust_chassis_gimbal_pos(pid_ctl_t *my_chassis[4], int16_t desired_yaw_angle, motor_t *yaw_motor) {
    int16_t deviation = get_angle_err(yaw_motor, desired_yaw_angle);
    int32_t rotate_speed = pid_calc(&chassis_rotate, deviation);

    for (int i = 0; i < 4; ++i) my_chassis[i]->motor->target += abs(direction) * rotate_speed;
}

void chassis_mode_forward(void) {
    direction = 1;
}

void chassis_mode_backward(void) {
    direction = -1;
}

void chassis_stop(void) {
    direction = 0;
}

void run_chassis(pid_ctl_t *my_chassis[4]){
    for (uint8_t i = 0; i < 4; ++i) my_chassis[i]->motor->out = pid_calc(my_chassis[i], my_chassis[i]->motor->target);
    set_motor_output(my_chassis[CHASSIS_FL]->motor, my_chassis[CHASSIS_FR]->motor,
                my_chassis[CHASSIS_RL]->motor, my_chassis[CHASSIS_RR]->motor);
}
