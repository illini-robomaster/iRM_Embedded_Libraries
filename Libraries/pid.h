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

enum{
    NOW    = 0,
    LAST   = 1,
    LLAST  = 2,
    LLLAST = 3,
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

    motor_t *motor; //motor it controls

    /* arrays that contain recent data (4 recent calls) */
    uint16_t *set; //target value
    uint16_t *err; //error
    int buffer_idx;

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
 * @brief  translate user-friendly input value into motor raw value
 *              (do nothing for now; edit it after finishing pid.)
 * @param  _input user input value e.g., desired angle
 * @return        corresponding raw value
 */
int motor_val_translate(float _input);

/**
 * @brief  calculate pid output given pid struct,
 *                              observed value and desired value. Designed for 3508 and 6623 motor
 * @param  my_motor desired pid struct
 * @param  desired_value target value
 * @return 1 or 0, where 0 means it doesn't modify anything (because constraints are not satisfied)
 */
int pid_calc(motor_pid_t *my_motor_pid, float desired_value);

extern motor_pid_t pid_pit;
extern motor_pid_t pid_yaw;
extern motor_pid_t pid_spd[4];
extern motor_pid_t pid_launcher[2];
extern motor_pid_t pid_poke;
extern motor_pid_t pid_rotation;

#endif
