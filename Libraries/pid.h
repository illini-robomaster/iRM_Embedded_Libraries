#ifndef _PID_H_
#define _PID_H_

#include "stm32f4xx_hal.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief Constrain candidate in [-ABS_MAX, ABS_MAX]
 * @param Pointer to candidate variable
 * @param ABS_MAX bound
 */
void abs_limit(float *candidate, float ABS_MAX);

enum{
    LLAST,
    LAST,
    NOW,
};

enum{
    POSITION_PID,
    DELTA_PID,
    GIMBAL_AUTO_SHOOT,
    GIMBAL_MAN_SHOOT,
    GIMBAL_MOUSE_IMU_SHOOT,
    POKE,
    CHASSISS_ROTATE
};

typedef struct __motor_pid_t
{
    float p;
    float i;
    float d;
    float v;

    /* arrays that contain recent data (3 calls) */
    float set[3]; //target value
    float get[3]; //observed(measured) value
    float err[3]; //error


    float pout; //calculated p output
    float iout; //calculated i output
    float dout; //calculated d output
    float vout; //calculated v output

    float pos_out; //new (current) position output (to be read by other program)
    float last_pos_out; //last position output
    float delta_u; //derivative of delta_out
    float delta_out; //new (current) delta output (to be read by other program) = last_delta_out + delta_u
    float last_delta_out; //last delta out

    float max_err;  //error threshold
    float deadband; //accuracy threshold. see https://en.wikipedia.org/wiki/PID_controller#Deadband
    uint32_t pid_mode; //for switch cases
    uint32_t MaxOutput; //output threshold; should be customized for each motor
    uint32_t IntegralLimit; //Integral limit
} motor_pid_t;

/**
 * helper function to init a motor_pid_t
 * @brief
 * @param pid             pid obj to be initialized (i.e., which motor to be configured)
 * @param mode            pid mode for this motor_pid
 *                              for explanation of mode, refer to header
 * @param maxout          Maximum output of this pid
 * @param intergral_limit Intergral limit
 * @param _p              user p
 * @param _i              user i
 * @param _d              user d
 * @param _v              user v
 */
void PID_struct_init( motor_pid_t* pid, uint32_t mode,  uint32_t maxout,
    uint32_t intergral_limit, float kp, float ki, float kd, float kv);

/**
 * @brief reset p/i/d/v config of desired motor pid
 * @param pid pointer to desired motor pid
 * @param kp  new p value
 * @param ki  new i value
 * @param kd  new d value
 * @param kv  new v value
 */
static void pid_reset(motor_pid_t *pid, float kp, float ki, float kd, float kv);

/**
 * @brief  calculate pid output given pid struct,
 *                              observed value and desired value. Designed for 3508 and 6623 motor
 * @param  pid desired pid object
 * @param  get current feedback value
 * @param  set target value
 * @return 1 or 0, where 0 means it doesn't modify anything (because constraints are not satisfied)
 */
int pid_calc(motor_pid_t* pid, float fdb, float ref);

extern motor_pid_t pid_pit;
extern motor_pid_t pid_yaw;
extern motor_pid_t pid_spd[4];
extern motor_pid_t pid_launcher[2];
extern motor_pid_t pid_poke;
extern motor_pid_t pid_rotation;

#endif
