#include "pid.h"
#include "motor.h"
#include "bsp_error_handler.h"
#include "rm_config.h"

static int16_t abs_limit(int16_t *data, int16_t lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

static float fabs_limit(float *data, float lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

static int16_t get_prev_n_err(pid_ctl_t *pid, uint8_t n) {
    return pid->err[(pid->idx + HISTORY_DATA_SIZE - n) % HISTORY_DATA_SIZE];
}

static float position_pid_calc(pid_ctl_t *pid) {
    int16_t err_now     = pid->err[pid->idx];
    int16_t err_last    = get_prev_n_err(pid, 1);

    if (!pid->int_rng || abs(err_now) < pid->int_rng)
        pid->integrator += err_now;
    if (pid->int_lim)
        abs_limit(&pid->integrator, pid->int_lim);
    
    float pout = pid->kp * err_now;
    float iout = pid->ki * pid->integrator;
    float dout = pid->kd * (err_now - err_last) / pid->dt;

    if (err_now - err_last > pid->max_derr)
        dout = 0;

    float final_out = pout + iout + dout;
    if (pid->maxout)
        fabs_limit(&final_out, pid->maxout);

    return final_out;
}

void pid_set_param(pid_ctl_t *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_init(pid_ctl_t *pid, pid_mode_t mode, motor_t *motor,
        int16_t low_lim, int16_t  high_lim, int16_t int_lim, int16_t int_rng, int16_t max_derr,
        float kp, float ki, float kd, float maxout, float dt) {
    pid->mode       = mode;
    pid->motor      = motor;
    pid->dt         = dt;
    pid->low_lim    = low_lim;
    pid->high_lim   = high_lim;
    pid->int_lim    = int_lim;
    pid->int_rng    = int_rng;
    pid->max_derr   = max_derr;
    pid->maxout     = maxout;
    pid->idx        = 0;
    pid->integrator = 0;
    pid_set_param(pid, kp, ki, kd);
}

int16_t pid_angle_ctl_angle(pid_ctl_t *pid, int16_t target_angle) {
    /* force target angle to be in range */
    if (clip_angle_err(pid->motor, target_angle - pid->low_lim) < 0)
        target_angle = pid->low_lim;
    else if (clip_angle_err(pid->motor, target_angle - pid->high_lim) > 0)
        target_angle = pid->high_lim;
    /* set angle error into the circular buffer */
    pid->idx = (++pid->idx) % HISTORY_DATA_SIZE;
    pid->err[pid->idx] = get_angle_err(pid->motor, target_angle);
    /* calculate generic posstion pid */
    return position_pid_calc(pid);
}

int16_t pid_speed_ctl_speed(pid_ctl_t *pid, int16_t target_speed) {
    /* force target speed to be in range */
    if (target_speed - pid->low_lim < 0)
        target_speed = pid->low_lim;
    else if (target_speed - pid->high_lim > 0)
        target_speed = pid->high_lim;
    /* set speed error into the circular buffer */
    pid->idx = (++pid->idx) % HISTORY_DATA_SIZE;
    pid->err[pid->idx] = get_speed_err(pid->motor, target_speed);
    /* calculate generic position pid */
    return position_pid_calc(pid);
}

int16_t pid_calc(pid_ctl_t *pid, int16_t target) {
    switch (pid->mode) {
        case GIMBAL_AUTO_SHOOT:
        case GIMBAL_MAN_SHOOT:
        case POKE:
            return pid_angle_ctl_angle(pid, target);
        case CHASSIS_ROTATE:
            return pid_speed_ctl_speed(pid, target);
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "pid mode does not exist");
            return 0;
    }
}
