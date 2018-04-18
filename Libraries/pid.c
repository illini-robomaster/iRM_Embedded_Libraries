#include "pid.h"
#include "motor.h"
#include "bsp_error_handler.h"

int16_t get_prev_n_err(pid_ctl_t *pid, uint8_t n) {
    return pid->err[(pid->idx + HISTORY_DATA_SIZE - n) % HISTORY_DATA_SIZE];
}

float position_pid_calc(pid_ctl_t *pid) {
    int16_t err_now     = pid->err[pid->idx];
    int16_t err_last    = get_prev_n_err(pid, 1);

    float pout = pid->kp * err_now;
    float iout = (pid->integrator += pid->ki * err_now);
    float dout = pid->kd * (err_now - err_last) / pid->dt;

    return pout + iout + dout;
}

float delta_pid_calc(pid_ctl_t *pid) {
    int16_t err_now     = get_prev_n_err(pid, 0);
    int16_t err_last    = get_prev_n_err(pid, 1);
    int16_t err_llast   = get_prev_n_err(pid, 2);

    /* TODO: should return the true output */
    return 0;
}

void pid_set_param(pid_ctl_t *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_init(pid_ctl_t *pid, pid_mode_t mode, motor_t *motor,
        float kp, float ki, float kd,
        float low_lim, float high_lim, float deadband, float dt) {
    pid->mode       = mode;
    pid->motor      = motor;
    pid->deadband   = deadband;
    pid->dt         = dt;
    pid->low_lim    = low_lim;
    pid->high_lim   = high_lim;
    pid->idx        = 0;
    pid->integrator = 0;
    pid_set_param(pid, kp, ki, kd);
}

void pid_angle_ctl_angle(pid_ctl_t *pid, int16_t target_angle) {
    /* force target angle to be in range */
    if (clip_angle_err(pid->motor, target_angle - pid->low_lim) < 0)
        target_angle = pid->low_lim;
    else if (clip_angle_err(pid->motor, target_angle - pid->high_lim) > 0)
        target_angle = pid->high_lim;
    /* set angle error into the circular buffer */
    pid->idx = (++pid->idx) % HISTORY_DATA_SIZE;
    pid->err[pid->idx] = get_angle_err(pid->motor, target_angle);
    /* calculate generic posstion pid */
    pid->motor->out = position_pid_calc(pid);
}

void pid_speed_ctl_speed(pid_ctl_t *pid, int16_t target_speed) {
    /* force target speed to be in range */
    if (target_speed - pid->low_lim < 0)
        target_speed = pid->low_lim;
    else if (target_speed - pid->high_lim > 0)
        target_speed = pid->high_lim;
    /* set speed error into the circular buffer */
    pid->idx = (++pid->idx) % HISTORY_DATA_SIZE;
    pid->err[pid->idx] = get_speed_err(pid->motor, target_speed);
    /* calculate generic position pid */
    pid->motor->out = position_pid_calc(pid);
}

uint8_t pid_calc(pid_ctl_t *pid, int16_t target) {
    switch (pid->mode) {
        case GIMBAL_AUTO_SHOOT:
        case GIMBAL_MAN_SHOOT:
        case POKE:
            pid_angle_ctl_angle(pid, target);
            return 1;
        case CHASSIS_ROTATE:
            pid_speed_ctl_speed(pid, target);
            return 1;
        default:
            bsp_error_handler(__FILE__, __LINE__, "pid mode does not exist");
            return 0;
    }
}
