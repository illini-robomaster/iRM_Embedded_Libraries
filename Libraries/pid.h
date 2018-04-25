/**
 * @file pid.h
 * @brief library level pid controller calculaation
 * @author Alvin Sun
 * @date 2018-04-18
 */
#ifndef _PID_H_
#define _PID_H_

#include "stm32f4xx_hal.h"
#include <math.h>
#include "motor.h"
#include <stdlib.h>

/**
 * @ingroup library
 * @defgroup pid PID
 * @{
 */

#define HISTORY_DATA_SIZE 4

/**
 * @enum pid_mode_t
 * @brief a enum type that defines motor usage
 * @var GIMBAL_AUTO_SHOOT   gimbal auto shooting mode
 * @var GIMBAL_MAN_SHOOT    gimbal manual shooting mode
 * @var POKE                bullet supply mode
 * @var CHASSIS_ROTATE      chassis motor mode
 */
typedef enum {
    GIMBAL_AUTO_SHOOT,
    GIMBAL_MAN_SHOOT,
    POKE,
    CHASSIS_ROTATE,
    FLYWHEEL,
}   pid_mode_t;

/**
 * @struct pid_ctl_t
 * @brief a pid controller type that stores all necessary information
 *        for regular pid controlling algorithm
 * @var kp          proportional constant
 * @var ki          integrative constant
 * @var kd          differentiative constant
 * @var maxout      maximum output
 * @var dt          time period of a pid calculation cycle in millisecond
 * @var mode        pid mode
 * @var motor       motor associated with this pid controller
 * @var err         a circular buffer for both latest and previous error value
 * @var idx         latest read index
 * @var integrator  error integrator
 * @var low_lim     lower limit for target value
 * @var high_lim    upper limit for target value
 * @var int_lim     integration limit
 * @var int_rng     integration range (integrator won't change out side the range)
 * @var max_derr    maximum error derivative (prevent discontinuity in set point sudden jump)
 */
typedef struct {
    float   kp;
    float   ki;
    float   kd;
    float   maxout;
    float   dt;
    float   deadband;

    pid_mode_t  mode;
    motor_t     *motor;

    uint8_t idx;
    int32_t err[HISTORY_DATA_SIZE];
    int32_t integrator;

    int32_t     low_lim;
    int32_t     high_lim;
    int32_t     int_lim;
    int32_t     int_rng;
    int32_t     max_derr;
    int32_t     ldata;
}   pid_ctl_t;

/**
 * @brief clip int32_t number into [-lim, lim]
 * @param data pointer to the number to be clipped
 * @param lim  absolute limit range
 * @return clipped value
 */
static int32_t abs_limit(int32_t *data, int32_t lim);

/**
 * @brief clip floating point number into [-lim, lim]
 * @param data pointer to the number to be clipped
 * @param lim  absolute limit range
 * @return clipped value
 */
static float fabs_limit(float *data, float lim);

/**
 * @brief get the nth latest error value (e.g. n = 0 means the latest error value)
 * @param pid   pid data structure
 * @param n     the nth latest
 * @return      the nth latest error value
 */
static int32_t get_prev_n_err(pid_ctl_t *pid, uint8_t n);

/**
 * @brief calculate generic position pid
 * @param pid pid data structure
 * @return calculated current output
 */
static float position_pid_calc(pid_ctl_t *pid);

/**
 * @brief initialize a pid controller instance
 * @param pid       pid controller pointer
 * @param mode      pid control mode
 * @param motor     associated motor instance
 * @param low_lim   target lower limit
 * @param high_lim  target upper limit
 * @param int_lim   integration limit [set to 0 to disable]
 * @param int_rng   range for enabling integration [set to 0 to disable]
 * @param max_derr  maximum error derivative [set to 0 to disable]
 * @param kp        proportional constant
 * @param ki        intergrative constant
 * @param kd        differentiative constant
 * @param maxout    maximum final out put [set to 0 to disable]
 * @param dt        delta t in millisecond
 * @return initialized pid controller pointer
 */
pid_ctl_t *pid_init(pid_ctl_t *pid, pid_mode_t mode, motor_t *motor, 
        int32_t low_lim, int32_t high_lim, int32_t int_lim, int32_t int_rng, int32_t max_derr,
        float kp, float ki, float kd, float maxout, float dt, float deadband);

/**
 * @brief set the p, i, d parameter of a pid controller
 * @param pid   pid controller
 * @param kp    porptional gain
 * @param ki    intergral gain
 * @param kd    derivative gain
 */
void pid_set_param(pid_ctl_t *pid, float kp, float ki, float kd);

/**
 * @brief use angle data from sensor to control angle
 * @param pid           pid controller
 * @param target_angle  target angle
 * @return calculated current output
 */
int32_t pid_angle_ctl_angle(pid_ctl_t *pid, int32_t target_angle);

/**
 * @brief use rotational speed data from sensor to control rotational speed
 * @param pid           pid controller
 * @param target_speed  target rotational speed
 * @return calculated current output
 */
int32_t pid_speed_ctl_speed(pid_ctl_t *pid, int32_t target_speed);

/**
 * @brief calculate pid value and set output to the motor
 * @param pid       pid instance
 * @param target    generic target value
 * @return calculated motor output
 */
int32_t pid_calc(pid_ctl_t *pid, int32_t target);

void pid_rotation_reset(pid_ctl_t *pid);

int32_t pid_rotation_ctl_rotation(pid_ctl_t *pid,
        int32_t *target, int32_t speed);

/** @} */

#endif
