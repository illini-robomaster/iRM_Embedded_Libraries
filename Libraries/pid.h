/**
 * @file pid.h
 * @brief library level pid controller calculaation
 * @author Alvin Sun
 * @date 2018-04-18
 */
#ifndef _PID_H_
#define _PID_H_

/**
 * @ingroup library
 * @defgroup pid PID
 * @{
 */

#include "stm32f4xx_hal.h"
#include <math.h>
#include "motor.h"
#include <stdlib.h>

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
    CHASSIS_ROTATE
}   pid_mode_t;

/**
 * @struct pid_ctl_t
 * @brief a pid controller type that stores all necessary information
 *        for regular pid controlling algorithm
 * @var kp          proportional constant
 * @var ki          integrative constant
 * @var kd          differentiative constant
 * @var integrator  error integrator
 * @var deadband    convergence condition (e.g. we consider target value achieved
 *                  when |err| < deadband)
 * @var dt          time period of a pid calculation cycle in millisecond
 * @var mode        pid mode
 * @var motor       motor associated with this pid controller
 * @var err         a circular buffer for both latest and previous error value
 * @var idx         latest read index
 * @var low_lim     lower limit for target value
 * @var high_lim    upper limit for target value
 */
typedef struct {
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

/**
 * @brief initialize a pid controller instance
 * @param pid       pid controller pointer
 * @param mode      pid control mode
 * @param motor     associated motor instance
 * @param kp        proportional constant
 * @param ki        intergrative constant
 * @param kd        differentiative constant
 * @param low_lim   target lower limit
 * @param high_lim  target upper limit
 * @param deadband  convergence condition
 * @param dt        delta t in millisecond
 */
void pid_init(pid_ctl_t *pid, pid_mode_t mode, motor_t *motor, 
        float kp, float ki, float kd,
        float low_lim, float high_lim, float deadband, float dt);

/**
 * @brief calculate pid value and set output to the motor
 * @param pid       pid instance
 * @param target    generic target value
 * @return 
 */
uint8_t pid_calc(pid_ctl_t *pid, int16_t target);

/** @} */

#endif
