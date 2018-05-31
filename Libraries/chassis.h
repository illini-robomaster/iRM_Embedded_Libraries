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
#define chs_ki 0.9f
#define chs_kd 0
#define chs_int_lim 200
#define chs_calc_interval 5

#define MAX_TURN_SPEED  1500
#define MAX_SPEED       2500
#define YAW_DEADBAND    0.1f

#define TURNING_SPEED   700

typedef enum{
    CHASSIS_FL = 0,
    CHASSIS_FR = 1,
    CHASSIS_RL = 2,
    CHASSIS_RR = 3
} chassis_motor_t;

/**
 * Initialize chassis motor pids
 * @brief
 * @param my_chassis ptr to the desired chassis object to be initialized
 */
void chassis_init(pid_ctl_t *my_chassis[4]);

/**
 * calculate chassis move based on keyboard input
 * @brief
 * @param my_chassis An (array) of motors pid that represent chassis
 * @param rc         dbus struct; for getting remote controller data
 * @param yaw_angle  Counterclockwise is positive. In radian. How far is gimbal deviate from chassis
 */
void calc_keyboard_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle);

/**
 * @brief calculate chassis move based on remote controller
 * @param my_chassis an (array) of motors pid that represent chassis
 * @param rc         dbus struct; for getting remote controller data
 * @param yaw_angle  angle of gimabl deviated from chassis, in radian. (counter colockwise is positive)
 */
void calc_remote_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle);

/**
 * @brief calculate chassis rotation based on remote controller
 * @param my_chassis an (array) of motors pid that represent chassis
 * @param rc         dbus struct; for getting remote controller data
 */
void calc_remote_rotate(pid_ctl_t *my_chassis[4], dbus_t *rc);

/**
 * @brief add rotation component to motor target
 * @param my_chassis an (array) of motors pid that represent chassis
 * @param speed      turning speed; postive for CCW direction
 */
void add_rotation(pid_ctl_t *my_chassis[4], float speed);

/**
 * Makes chassis roate based on yaw motor feedback
 * @TODO: use P controller instead
 * @brief
 * @param my_chassis my chassis object. An array of pid that represents chassis
 * @param yaw_angle  yaw angle from yaw motor feedback
 */
void calc_gimbal_compensate(pid_ctl_t *my_chassis[4], float yaw_angle);

/**
 * Run chassis motors. SHOULD ONLY BE CALLED AFTER PID CALC
 * @brief
 * @param my_chassis my chassis object. An array of pid that represents chassis
 */
void run_chassis(pid_ctl_t *my_chassis[4]);

#endif
