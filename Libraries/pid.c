#include "pid.h"
#include "motor.h"
#include "bsp_error_handler.h"
#include "rm_config.h"
#include "utils.h"

static int32_t abs_limit(int32_t *data, int32_t lim) {
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

static int32_t get_prev_n_err(pid_ctl_t *pid, uint8_t n) {
    return pid->err[(pid->idx + HISTORY_DATA_SIZE - n) % HISTORY_DATA_SIZE];
}

static float position_pid_calc(pid_ctl_t *pid) {
    int32_t err_now     = pid->err[pid->idx];
    int32_t err_last    = get_prev_n_err(pid, 1);

    if (!pid->int_rng || abs(err_now) < pid->int_rng)
        pid->integrator += err_now;
    if (pid->int_lim)
        abs_limit(&pid->integrator, pid->int_lim);
    if (pid->deadband && abs(err_now) < pid->deadband)
        err_now = 0;

    float pout = pid->kp * err_now;
    float iout = pid->ki * pid->integrator;
    float dout = pid->kd * (err_now - err_last);

    if (pid->max_derr && abs(err_now - err_last) > pid->max_derr)
        dout = 0;

    float final_out = pout + iout + dout;
    if (pid->maxout)
        fabs_limit(&final_out, pid->maxout);

    return final_out;
}

int32_t default_model(void *args) {
    return 0;
}

void pid_set_param(pid_ctl_t *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

pid_ctl_t *pid_init(pid_ctl_t *pid, pid_mode_t mode, motor_t *motor,
        int32_t low_lim, int32_t  high_lim, int32_t int_lim, int32_t int_rng, int16_t max_derr,
        float kp, float ki, float kd, float maxout, float deadband) {
    if (!pid)
        pid = pvPortMalloc(sizeof(pid_ctl_t));
    get_motor_data(motor);
    pid->mode       = mode;
    pid->motor      = motor;
    pid->deadband   = deadband;
    pid->low_lim    = low_lim;
    pid->high_lim   = high_lim;
    pid->int_lim    = int_lim;
    pid->int_rng    = int_rng;
    pid->maxout     = maxout;
    pid->idx        = 0;
    pid->integrator = 0;
    pid->prev_tar   = get_motor_angle(motor);
    pid->max_derr   = max_derr;
    pid->model      = default_model;
    pid->model_args = NULL;
    pid_set_param(pid, kp, ki, kd);
    for (int i = 0; i < HISTORY_DATA_SIZE; ++i) { pid->err[i] = 0; }
    return pid;
}

void pid_set_model(pid_ctl_t *pid, int32_t (*model)(void *)) {
    pid->model = model;
}

int32_t pid_manual_error(pid_ctl_t *pid, int32_t manual_error) {
    pid->idx = (++pid->idx) % HISTORY_DATA_SIZE;
    pid->err[pid->idx] = manual_error;
    return position_pid_calc(pid);
}

int32_t pid_angle_ctl_angle(pid_ctl_t *pid, int32_t target_angle) {
    /* force target angle to be in range */
    if (pid->low_lim && clip_angle_err(pid->motor, target_angle - pid->low_lim) < 0)
        target_angle = pid->low_lim;
    else if (pid->high_lim && clip_angle_err(pid->motor, target_angle - pid->high_lim) > 0)
        target_angle = pid->high_lim;
    /* set angle error into the circular buffer */
    pid->idx = (++pid->idx) % HISTORY_DATA_SIZE;
    /* ramping control not quite efficient
    if (pid->step_size) {
        if (clip_angle_err(pid->motor, target_angle - pid->prev_tar) > 0)
            target_angle = min(pid->prev_tar + pid->step_size, target_angle);
        else
            target_angle = max(pid->prev_tar - pid->step_size, target_angle);
    } */
    pid->prev_tar = target_angle;
    pid->err[pid->idx] = get_angle_err(pid->motor, target_angle);
    /* calculate generic position pid */
    return position_pid_calc(pid);
}

int32_t pid_speed_ctl_speed(pid_ctl_t *pid, int32_t target_speed) {
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

int32_t pid_calc(pid_ctl_t *pid, int32_t target) {
    get_motor_data(pid->motor);
    switch (pid->mode) {
        case GIMBAL_AUTO_SHOOT:
        case GIMBAL_MAN_SHOOT:
            return pid_angle_ctl_angle(pid, target) + \
                pid->model(pid->model_args);
        case CHASSIS_ROTATE:
        case FLYWHEEL:
        case POKE:
            return pid_speed_ctl_speed(pid, target) + \
                pid->model(pid->model_args);
        case MANUAL_ERR_INPUT:
            return pid_manual_error(pid, target) + \
                pid->model(pid->model_args);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "pid mode does not exist");
            return 0;
    }
}
