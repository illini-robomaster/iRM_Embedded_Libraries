#ifndef _SHOOTER_H_
#define _SHOOTER_H_

#include "pid.h"
#include "motor.h"
#include "FreeRTOS.h"

typedef enum {
    CAN,
    PWM,
}   shooter_type_t;

typedef enum {
    FLYWHL_ON,
    FLYWHL_OFF,
}   flywhl_state_t;

typedef struct {
    motor_t     *left;
    motor_t         *right;
    flywhl_state_t  state;
}   flywhl_t;

typedef struct {
    flywhl_t        *flywhl;
    pid_ctl_t       *poker;
    shooter_type_t  type;
}   shooter_t;

static flywhl_t* pwm_flywhl_init(flywhl_t *pwm_flywhl);

static flywhl_t* can_flywhl_init(flywhl_t *can_flywhl);

static pid_ctl_t* poker_init(pid_ctl_t* poker);

shooter_t* shooter_init(shooter_t *shooter, shooter_type_t type);

void flywhl_set_output(shooter_t *shooter, int32_t output);

void flywhl_on(shooter_t *shooter);

void flywhl_off(shooter_t *shooter);

void poker_set_speed(shooter_t *shooter, int32_t speed);

#endif
