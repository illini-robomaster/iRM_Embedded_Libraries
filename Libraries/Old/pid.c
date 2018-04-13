/******************************************************************************
/// @brief
/// @copyright Copyright (c) 2017 <dji-innovations, Corp. RM Dept.>
/// @license MIT License
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction,including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense,and/or sell
/// copies of the Software, and to permit persons to whom the Software is furnished
/// to do so,subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
*******************************************************************************/
/**
  ******************************************************************************
  * @file			pid.c
  * @version		V1.0.1
  * @date			11/24/17
  * @brief   		Actual output may be declared as get/measure/real/fdb
			        Expected output may be declared as set/target/ref
  *****************************************************************************/

#include "pid.h"

/******************************************************************************
	Input
        get, set, err
	Output
        pos_out
	Description
        For M3508 P19 Motor with C620 ESC
        Use PID to rectify real-time errors
        Reach expectd position within minimum time and least oscillations
	Author
        NL 11/24/17, SC 11/24/17, YM 11/24/17
*******************************************************************************/
void abs_limit(float *a, float ABS_MAX) {
    if(*a > ABS_MAX)
       *a = ABS_MAX;
    if(*a < -ABS_MAX)
       *a = -ABS_MAX;
} //Constrain variable in [-ABS_MAX, ABS_MAX]

/***********************************************************
 * purpose: private function to set PID private value
 * last change:
 *      Jeff Ma 2017/12/20 add comments
 *      Steven Chan 2018/01/04 Add Unparametrized Adaptive PID
 **********************************************************/

static void pid_param_init(
    motor_pid_t *pid, // Determine ID of motor affected by PID
    uint32_t mode, // Determine mode of PID
    uint32_t maxout, // Maxium output by PID
    uint32_t intergral_limit, // Limit of I

    float 	kp,
    float 	ki,
    float 	kd,
    float   kv) {

    pid->IntegralLimit = intergral_limit;
    pid->MaxOutput = maxout;
    pid->pid_mode = mode;

    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    pid->v = kv;

    /*set the default value for deadband and max_err*/
    pid->deadband = 1.0f; // set PID output tolerance
    pid->max_err = maxout; // set error upper bound

}

/**************************************************************
 *  Purpose: reset kp, ki, kd, mode
 *  last change:
 *      Jeff Ma     2017/12/20
 *      Steve Chan  2018/01/04
 **************************************************************/

static void pid_reset(motor_pid_t *pid, float kp, float ki, float kd, float kv) {
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    pid->v = kv;
}

/****************************************************************************

    purpose: PID calculation
    input  : PID subject, real time beedback, target goal
    output : give PID output
    caller : 3508 motor, 6623 motor
    last chagne:
        Jeff Ma     2017/12/20/  gimbal PID for auto shooting
        Steven Chan 2018/01/04/  Add unparametrized adaptive PID for all modes

*****************************************************************************/
float pid_calc(motor_pid_t* pid, float get, float set) {
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get;	//expected - measured

    /* Basic position PID
     * Used for chassis motor, gimbal auto-shooting mode
     */
    if(pid->pid_mode == POSITION_PID) //Calculate Position PID
    {
        // modified
        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
            return 0;
        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
            return 0;
        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output

        pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
        pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
        pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; //Update last time
    }
    /* Basic delta PID
     * Used for gimbal man-shooting mode
     */
    else if(pid->pid_mode == DELTA_PID)//Calculate Delta PID
    {
        if (pid->err[NOW] >= 8191/2)
            pid->err[NOW] = pid->err[NOW] - 8191;	//expected - measured
        else if (pid->err[NOW] <= -8191/2)
            pid->err[NOW] = pid->err[NOW] + 8191;	//expected - measured

        pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]); //pout=kp*d(err)/dt
        pid->iout = pid->i * pid->err[NOW]; //iout=ki*err
        pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]); //dout=kd*d(d(err)/dt)/dt
        pid->vout = pid->v * (pid->get[NOW] - 2*pid->get[LAST] + pid->get[LLAST]); //vout=kv*a_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->delta_u = pid->pout + pid->iout + pid->dout - pid->vout; //delta_u=pout+iout+dout-vout
        pid->delta_out = pid->last_delta_out + pid->delta_u; //delta_out=last_delta_out+delta_u
        abs_limit(&(pid->delta_out), pid->MaxOutput);
        pid->last_delta_out = pid->delta_out;

        // modified
        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
            pid->delta_out = 0;
        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
            pid->delta_out = 0;
        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output
    }
    //for gimbal auto-shooting mode
    else if(pid->pid_mode == GIMBAL_AUTO_SHOOT){

        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
            return 0;
        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
            return 0;
        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output

        if (pid->err[NOW] >= 8191/2)
            pid->err[NOW] = pid->err[NOW] - 8191;	//expected - measured
        else if (pid->err[NOW] <= -8191/2)
            pid->err[NOW] = pid->err[NOW] + 8191;	//expected - measured

        pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
        pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
        pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; //Update last time

    }
    //for gimbal man-shooting mode
    else if(pid->pid_mode == GIMBAL_MAN_SHOOT){

        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
            return 0;
        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
            return 0;
        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output
        if (pid->err[NOW] >= 8191/2)
            pid->err[NOW] = pid->err[NOW] - 8191;	//expected - measured
        else if (pid->err[NOW] <= -8191/2)
            pid->err[NOW] = pid->err[NOW] + 8191;	//expected - measured

        pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
        pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
        pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; //Update last time

    }
        //for gimbal mouse imu mode
    else if(pid->pid_mode == GIMBAL_MOUSE_IMU_SHOOT){

        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
            return 0;
        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
            return 0;
        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output
        if (pid->err[NOW] >= 8191/2)
            pid->err[NOW] = pid->err[NOW] - 8191;	//expected - measured
        else if (pid->err[NOW] <= -8191/2)
            pid->err[NOW] = pid->err[NOW] + 8191;	//expected - measured

        pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
        pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
        pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; //Update last time

    }
    else if(pid->pid_mode == CHASSISS_ROTATE){

        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
            return 0;
        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
            return 0;
        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output

        pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
        pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
        pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; //Update last time

    }
        //for gimbal mouse imu mode
    else if(pid->pid_mode == GIMBAL_MOUSE_IMU_SHOOT){

        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
            return 0;
        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
            return 0;
        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output
        if (pid->err[NOW] >= 8191/2)
            pid->err[NOW] = pid->err[NOW] - 8191;	//expected - measured
        else if (pid->err[NOW] <= -8191/2)
            pid->err[NOW] = pid->err[NOW] + 8191;	//expected - measured

        pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
        pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
        pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; //Update last time

    }
    else if(pid->pid_mode == POKE){

//			if (pid->err[NOW] >= 8191/2){
//            pid->err[NOW] = pid->err[NOW] - 8191;	//expected - measured
//			}
//        else if (pid->err[NOW] <= -8191/2){
//            pid->err[NOW] = pid->err[NOW] + 8191;	//expected - measured
//			}
//			if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err){
//            return 0;
//        } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
//        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband){
//            return 0;
//        } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output

    pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
    pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
    pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
    pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
    abs_limit(&(pid->iout), pid->IntegralLimit);
    pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
    abs_limit(&(pid->pos_out), pid->MaxOutput);
    pid->last_pos_out = pid->pos_out; //Update last time
    }
    else
    {
        return 0;
    }

    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    return pid->pid_mode==POSITION_PID ? pid->pos_out : pid->delta_out;

}

/**
    *@bref. Special calculation of position PID using GYRO
    *@param[in] get: real measure
    *@param[in] set：target

float pid_sp_calc(motor_pid_t* pid, float get, float set, float gyro) {
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get;	//set - measure

    if(pid->pid_mode == POSITION_PID) //Position PID
    {
        pid->pout = pid->p * pid->err[NOW];
		if(fabs(pid->i) >= 0.001f) //Apply I if |ki|>=0.001
			pid->iout += pid->i * pid->err[NOW];
		else
			pid->iout = 0;
        pid->dout = -pid->d * gyro/100.0f; //dout=-kd*gyro/100
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout;
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out;	//update last time
    }
    else if(pid->pid_mode == DELTA_PID)//增量式P
    {
//        pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
//        pid->iout = pid->i * pid->err[NOW];
//        pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);
//
//        abs_limit(&(pid->iout), pid->IntegralLimit);
//        pid->delta_u = pid->pout + pid->iout + pid->dout;
//        pid->delta_out = pid->last_delta_out + pid->delta_u;
//        abs_limit(&(pid->delta_out), pid->MaxOutput);
//        pid->last_delta_out = pid->delta_out;	//update last time
    }

    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    return pid->pid_mode==POSITION_PID ? pid->pos_out : pid->delta_out;

}*/

/************************************************************************
 * purpose: set PID private variable
 * last modified:
 *      Jeff Ma     2017/12/20  add comments
 *      Steven CHan 2018/01/04  Add kv
 ***********************************************************************/

void PID_struct_init(
    motor_pid_t* pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,

    float 	kp,
    float 	ki,
    float 	kd,
    float   kv) {
/*Initialize function pointer*/
    pid->f_param_init = pid_param_init;
    pid->f_pid_reset = pid_reset;
//	pid->f_cal_pid = pid_calc;
//	pid->f_cal_sp_pid = pid_sp_calc;	//addition

/*Initialize pid param */
    pid->f_param_init(pid, mode, maxout, intergral_limit, kp, ki, kd, kv);

}


// for modeling the electrical exis PIDs
//motor_pid_t pid_rol = {0};
//motor_pid_t pid_pit = {0};
//motor_pid_t pid_yaw = {0};
//motor_pid_t pid_yaw_omg = {0};//角速度环
//motor_pid_t pid_pit_omg = {0};//角速度环
//motor_pid_t pid_yaw_alfa = {0};		//Angle acceleration

//motor_pid_t pid_chassis_angle={0};
//motor_pid_t pid_poke = {0};
//motor_pid_t pid_poke_omg = {0};
//motor_pid_t pid_imu_tmp;
//motor_pid_t pid_x;
//motor_pid_t pid_cali_bby;
//motor_pid_t pid_cali_bbp;

motor_pid_t pid_pit;
motor_pid_t pid_yaw;
motor_pid_t pid_spd[4];
motor_pid_t pid_launcher[2];
motor_pid_t pid_poke;
motor_pid_t pid_rotation;

void pid_test_init() {


//为了解决上位机调参的时候第一次赋值的时候清零其他参数， 应该提前把参数表填充一下！

}
