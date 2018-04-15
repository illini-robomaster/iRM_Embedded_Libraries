#include "pid.h"

/* explicitly declare extern variables */
motor_pid_t pid_pit;
motor_pid_t pid_yaw;
motor_pid_t pid_spd[4];
motor_pid_t pid_launcher[2];
motor_pid_t pid_poke;
motor_pid_t pid_rotation;

void abs_limit(float *candidate, float ABS_MAX) {
    if(*a > ABS_MAX){
        *a = ABS_MAX;
        return;
    }
    if(*a < -ABS_MAX){
        *a = -ABS_MAX;
    }
    return;
}

static void pid_reset(motor_pid_t *pid, float kp, float ki, float kd, float kv) {
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    pid->v = kv;
}

int pid_calc(motor_pid_t* pid, float get, float set) {
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
    //use speed to control pos.
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
    // rune
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
    // manual shoot (from client)
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
    // correct version! man shoot (with IMU calibration)
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
    // use chassis motor input to calibrate motor speed
    else if(pid->pid_mode == CHASSISS_ROTATE){
        // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
        if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err)
            return 0;
        // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output
        if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband)
            return 0;
        pid->pout = pid->p * pid->err[NOW]; //pout=kp*err
        pid->iout += pid->i * pid->err[NOW]; //iout=sum[ki*err]
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]); //dout=kd*d(err)/dt
        pid->vout = pid->v * (pid->get[NOW] - pid->get[LAST]); //vout=kv*v_get
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->pos_out = pid->pout + pid->iout + pid->dout - pid->vout; //pos_out=pout+iout+dout-vout
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out; //Update last time
    }
    // poker PID
    else if(pid->pid_mode == POKE){
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
        //mode not supported?
        return 0;
    }
    /* update pid history regardless of pid mode */
    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    return 1;
    //return pid->pid_mode==POSITION_PID ? pid->pos_out : pid->delta_out;
}

void PID_struct_init(motor_pid_t *pid, uint32_t mode, uint32_t maxout,
        uint32_t intergral_limit, float _p, float _i, float _d, float _v) {
    /*Initialize pid param */
    pid->IntegralLimit = intergral_limit;
    pid->MaxOutput = maxout;
    pid->pid_mode = mode;

    pid->p = _p;
    pid->i = _i;
    pid->d = _d;
    pid->v = _v;

    /*set the default value for deadband and max_err*/
    pid->deadband = 1.0f; // set PID output tolerance
    pid->max_err = maxout; // set error upper bound
}
