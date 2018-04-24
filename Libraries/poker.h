#ifndef _POKER_H_
#define _POKER_H_

#include "stm32f4xx_hal.h"
#include <math.h>
#include "motor.h"
#include <stdlib.h>
#include "pid.h"

#define AVG_ANGLE_SUM 18500
#define NORMAL_SPEED  3000
#define RESIST_THRES  10000

typedef struct {
    float   kp;
    float   ki;
    motor_t     *motor;

    int integrator;

    int16_t     low_lim;
    int16_t     high_lim;
}  poker_ctl_t;

void poker_init(poker_ctl_t *my_poker, float _kp, float _ki, motor_t *_motor);

void single_shot(poker_ctl_t *my_poker);

#endif
