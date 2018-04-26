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

    my_chassis->pid_fl = pid_fl;
    my_chassis->pid_fr = pid_fr;
    my_chassis->pid_rl = pid_rl;
    my_chassis->pid_rr = pid_rr;

    // init CAN
    int iter = 0;
    while (iter < 100) {
        get_motor_data(my_chassis->pid_fl->motor);
        my_chassis->pid_fl->motor->out = 1;
        set_motor_output(my_chassis->pid_fl->motor, my_chassis->pid_fl->motor,
            my_chassis->pid_fl->motor, my_chassis->pid_fl->motor);
        HAL_Delay(10);
        ++iter;
    }
}

void calc_chassis_output(chassis_t *my_chassis, float normalized_desired_speed,
        float desired_angle, float normalized_change_rate){
    float v_fl = MAX_SPEED * (normalized_desired_speed * sin(desired_angle + Q_PI) + normalized_change_rate);
    float v_fr = -MAX_SPEED * (normalized_desired_speed * cos(desired_angle + Q_PI) - normalized_change_rate);
    float v_rl = MAX_SPEED * (normalized_desired_speed * cos(desired_angle + Q_PI) + normalized_change_rate);
    float v_rr = -MAX_SPEED * (normalized_desired_speed * sin(desired_angle + Q_PI) - normalized_change_rate);

    my_chassis->pid_fl->motor->out = pid_calc(my_chassis->pid_fl, v_fl);
    my_chassis->pid_fr->motor->out = pid_calc(my_chassis->pid_fr, v_fr);
    my_chassis->pid_rl->motor->out = pid_calc(my_chassis->pid_rl, v_rl);
    my_chassis->pid_rr->motor->out = pid_calc(my_chassis->pid_rr, v_rr);
}

void run_chassis(chassis_t *my_chassis){
    set_motor_output(my_chassis->pid_fl->motor, my_chassis->pid_fr->motor,
                my_chassis->pid_rl->motor, my_chassis->pid_rr->motor);
}
