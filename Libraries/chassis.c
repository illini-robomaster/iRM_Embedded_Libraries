#include "chassis.h"
#include <math.h>
#include <stdlib.h>

void chassis_init(chassis_t *my_chassis){
    motor_t *m_fl, *m_fr, *m_rl, *m_rr;
    pid_ctl_t *pid_fl, *pid_fr, *pid_rl, *pid_rr;
    m_fl = m_fr = m_rl = m_rr = NULL;
    pid_fl = pid_fr = pid_rl = pid_rr = NULL;

    m_fl = motor_init(NULL, FL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_fr = motor_init(NULL, FR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rl = motor_init(NULL, RL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rr = motor_init(NULL, RR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);

    pid_fl = pid_init(NULL, CHASSIS_ROTATE, m_fl, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);
    pid_fr = pid_init(NULL, CHASSIS_ROTATE, m_fr, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);
    pid_rl = pid_init(NULL, CHASSIS_ROTATE, m_rl, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);
    pid_rr = pid_init(NULL, CHASSIS_ROTATE, m_rr, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);

    my_chassis[CHASSIS_FL] = pid_fl;
    my_chassis[CHASSIS_FR] = pid_fr;
    my_chassis[CHASSIS_RL] = pid_rl;
    my_chassis[CHASSIS_RR] = pid_rr;
}

void calc_keyboard_move(chassis_t *my_chassis, dbus_t *rc, float yaw_angle) {
    // clockwise
    yaw_angle = yaw_angle + Q_PI;
    float v_y = 0;
    float v_x = 0;
    if (rc->key.bit.W)
        v_y += 1;
    if (rc->key.bit.S)
        v_y -= 1;
    if (rc->key.bit.A)
        v_x -= 1;
    if (rc->key.bit.D)
        v_x += 1;
    float norm = 1;
    if(v_x != 0 || v_y != 0) {
        norm = sqrt(v_x * v_x + v_y * v_y);
    }
    v_x /= norm;
    v_y /= norm;
    // rotation; change of basis matrix.
    float out_x = (v_x * cos(yaw_angle) + v_y * sin(yaw_angle)) * MAX_SPEED;
    float out_y = (-v_x * sin(yaw_angle) + v_y * cos(yaw_angle)) * MAX_SPEED;
    my_chassis[CHASSIS_FL]->motor->out = out_x;
    my_chassis[CHASSIS_RR]->motor->out = -out_x; // velocity is the same. It's just these two motors are installed in opposite direction.
    my_chassis[CHASSIS_RL]->motor->out = out_y;
    my_chassis[CHASSIS_FR]->motor->out = -out_y;
}

void calc_gimbal_compensate(chassis_t *my_chassis, float yaw_angle) {
    if (fabsf(yaw_angle) < YAW_DEADBAND)
        return;
    if (yaw_angle > 0) {
        // turn clockwise
        my_chassis[CHASSIS_FL]->motor->out += TURNING_SPEED;
        my_chassis[CHASSIS_RR]->motor->out += -TURNING_SPEED; // velocity is the same. It's just these two motors are installed in opposite direction.
        my_chassis[CHASSIS_RL]->motor->out += TURNING_SPEED;
        my_chassis[CHASSIS_FR]->motor->out += -TURNING_SPEED;
    } else {
        my_chassis[CHASSIS_FL]->motor->out += -TURNING_SPEED;
        my_chassis[CHASSIS_RR]->motor->out += TURNING_SPEED;
        my_chassis[CHASSIS_RL]->motor->out += -TURNING_SPEED;
        my_chassis[CHASSIS_FR]->motor->out += TURNING_SPEED;
    }
}

void run_chassis(chassis_t *my_chassis){
    for (int i = 0; i < 4; ++i) {
        my_chassis[i]->motor->out = pid_calc(my_chassis[i], my_chassis[i]->motor->out);
    }
    set_motor_output(my_chassis[CHASSIS_FL]->motor, my_chassis[CHASSIS_FR]->motor,
                my_chassis[CHASSIS_RL]->motor, my_chassis[CHASSIS_RR]->motor);
}
