#include "chassis.h"
#include <math.h>

void chassis_init(chassis_t *my_chassis){
    motor_t m_fl, m_fr, m_rl, m_rr;
    pid_ctl_t pid_fl, pid_fr, pid_rl, pid_rr;

    motor_init(&m_fl, FL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    motor_init(&m_fr, FR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    motor_init(&m_rl, RL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    motor_init(&m_rr, RR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);

    pid_init(&pid_fl, CHASSIS_ROTATE, &m_fl, -MAX_SPEED, MAX_SPEED, int_lim, 0, 0, kp, ki, kd, 0, calc_interval, 0);
    pid_init(&pid_fr, CHASSIS_ROTATE, &m_fr, -MAX_SPEED, MAX_SPEED, int_lim, 0, 0, kp, ki, kd, 0, calc_interval, 0);
    pid_init(&pid_rl, CHASSIS_ROTATE, &m_rl, -MAX_SPEED, MAX_SPEED, int_lim, 0, 0, kp, ki, kd, 0, calc_interval, 0);
    pid_init(&pid_rr, CHASSIS_ROTATE, &m_rr, -MAX_SPEED, MAX_SPEED, int_lim, 0, 0, kp, ki, kd, 0, calc_interval, 0);

    my_chassis->pid_fl = &pid_fl;
    my_chassis->pid_fr = &pid_fr;
    my_chassis->pid_rl = &pid_rl;
    my_chassis->pid_rr = &pid_rr;

    // init CAN
    for (uint8_t i = 0; i < 100; i++) {
        get_motor_data(my_chassis->pid_fl->motor);
        m_fl.out = 1;
        set_motor_output(my_chassis->pid_fl->motor, my_chassis->pid_fl->motor,
            my_chassis->pid_fl->motor, my_chassis->pid_fl->motor);
    }
}

void calc_chassis_output(chassis_t *my_chassis, float normalized_desired_speed,
        float desired_angle, float normalized_change_rate){
    float v_fl = MAX_SPEED * (normalized_desired_speed * sin(desired_angle + Q_PI) + normalized_change_rate);
    float v_fr = MAX_SPEED * (normalized_desired_speed * cos(desired_angle + Q_PI) - normalized_change_rate);
    float v_rl = MAX_SPEED * (normalized_desired_speed * cos(desired_angle + Q_PI) + normalized_change_rate);
    float v_rr = MAX_SPEED * (normalized_desired_speed * sin(desired_angle + Q_PI) - normalized_change_rate);

    my_chassis->pid_fl->motor->out = pid_calc(my_chassis->pid_fl, v_fl);
    my_chassis->pid_fr->motor->out = pid_calc(my_chassis->pid_fr, v_fr);
    my_chassis->pid_rl->motor->out = pid_calc(my_chassis->pid_rl, v_rl);
    my_chassis->pid_rr->motor->out = pid_calc(my_chassis->pid_rr, v_rr);
}

void run_chassis(chassis_t *my_chassis){
    set_motor_output(my_chassis->pid_fl->motor, my_chassis->pid_fr->motor,
                my_chassis->pid_rl->motor, my_chassis->pid_rr->motor);
}
