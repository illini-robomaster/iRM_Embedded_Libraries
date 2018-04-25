#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "pid.h"
#include "motor.h"

#define MAX_SPEED 1000
#define Q_PI 0.7853982 //pi / 4

#define CHASSIS_MOTOR_TYPE M3508

#define FL_MOTOR_CANID 0x201 // front left motor can id
#define FR_MOTOR_CANID 0x202
#define RL_MOTOR_CANID 0x203
#define RR_MOTOR_CANID 0x204

/* empirical parameters */
#define chs_kp 10
#define chs_ki 0.9
#define chs_kd 0
#define chs_int_lim 200
#define chs_calc_interval 5

typedef struct __chassis {
    pid_ctl_t *pid_fl;
    pid_ctl_t *pid_fr;
    pid_ctl_t *pid_rl;
    pid_ctl_t *pid_rr;
} chassis_t;

void chassis_init(chassis_t *my_chassis);

void calc_chassis_output(chassis_t *my_chassis, float normalized_desired_speed,
        float desired_angle, float normalized_change_rate);

void run_chassis(chassis_t *my_chassis);

#endif
