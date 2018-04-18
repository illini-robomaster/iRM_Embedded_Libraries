#ifndef _PID_H_
#define _PID_H_

#include "stm32f4xx_hal.h"
#include <math.h>
#include "motor.h"
#include <stdlib.h>

/**
 * @brief Constrain candidate in [-ABS_MAX, ABS_MAX]
 * @param Pointer to candidate variable
 * @param ABS_MAX bound
 */
void abs_limit(float *candidate, float ABS_MAX);

#define HISTORY_DATA_SIZE 4

typedef enum {
    GIMBAL_AUTO_SHOOT,
    GIMBAL_MAN_SHOOT,
    GIMBAL_MOUSE_IMU_SHOOT,
    POKE,
    CHASSISS_ROTATE
}   pid_mode_t;

typedef struct pid_ctl_s
{
    float   kp;
    float   ki;
    float   kd;
    float   integrator;
    float   deadband;
    float   dt;

    pid_mode_t  mode;
    motor_t     *motor;

    int16_t err[HISTORY_DATA_SIZE];
    uint8_t idx;

    int16_t     low_lim;
    int16_t     high_lim;
}   pid_ctl_t;

void pid_init(pid_ctl_t *pid, pid_mode_t mode, motor_t *motor, 
        float kp, float ki, float kd,
        float low_lim, float high_lim, float deadband, float dt);

uint8_t pid_calc(pid_ctl_t *pid, int16_t target);


#endif
