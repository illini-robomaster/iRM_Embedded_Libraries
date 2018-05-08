#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "pid.h"
#include "motor.h"
#include "dbus.h"

#define Q_PI 0.7853982 // \pi / 4

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

#define TURNING_SPEED 500
#define MAX_SPEED     2000
#define YAW_DEADBAND  0.03

typedef enum{
    CHASSIS_FL = 0,
    CHASSIS_FR = 1,
    CHASSIS_RL = 2,
    CHASSIS_RR = 3
} chassis_motor_t;

typedef pid_ctl_t* chassis_t;

void chassis_init(chassis_t *my_chassis);

/**
 * [calc_keyboard_move description]
 * @brief
 * @param my_chassis [description]
 * @param rc         [description]
 * @param yaw_angle  Clockwise
 */
void calc_keyboard_move(chassis_t *my_chassis, dbus_t *rc, float yaw_angle);

void calc_gimbal_compensate(chassis_t *my_chassis, float yaw_angle);

void run_chassis(chassis_t *my_chassis);

#endif
