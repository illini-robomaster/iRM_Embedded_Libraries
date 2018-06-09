#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "pid.h"
#include "motor.h"
#include "dbus.h"
#include "referee.h"
#include "lib_config.h"

#define Q_PI 0.7853982 // \pi / 4

#define CHASSIS_MOTOR_TYPE M3508

#define FL_MOTOR_CANID 0x201 // front left motor can id
#define FR_MOTOR_CANID 0x202
#define RL_MOTOR_CANID 0x203
#define RR_MOTOR_CANID 0x204


#define ROTATE_KP 2.7f

#define EVASIVE_DEADBAND 400
#define YAW_DEADBAND     50 // 22 ~ 1 deg

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
 * Makes chassis roate based on yaw motor feedback
 * @brief
 * @param my_chassis my chassis object. An array of pid that represents chassis
 * @param desired_yaw_angle desired relative yaw angle;
 * @param yaw_motor ptr to yaw motor for getting angle feedback.
 */
void adjust_chassis_gimbal_pos(pid_ctl_t *my_chassis[4], int16_t desired_yaw_angle, motor_t *yaw_motor);

//TODO for Roger: finish this god damn doc
/**
 * [evasive_move description]
 * @brief
 * @param [name]           array of motors that control chassis
 * @param cur_yaw_feedback current yaw feedback read from yaw motor
 * @param yaw_motor        ptr to yaw motor
 */
void evasive_move(pid_ctl_t *my_chassis[4], int16_t cur_yaw_feedback, motor_t *yaw_motor);

/**
 * @brief set chassis to forward mode
 */
void chassis_mode_forward(void);

/**
 * @brief set chassis to backward mode
 */
void chassis_mode_backward(void);

/**
 * @brief disable all chassis related movement
 */
void chassis_stop(void);

/**
 * Run chassis motors. SHOULD ONLY BE CALLED AFTER PID CALC
 * @brief
 * @param my_chassis my chassis object. An array of pid that represents chassis
 */
void run_chassis(pid_ctl_t *my_chassis[4]);

#endif
