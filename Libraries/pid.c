#include "pid.h"

/* explicitly declare extern variables */
motor_pid_t pid_pit;
motor_pid_t pid_yaw;
motor_pid_t pid_spd[4];
motor_pid_t pid_launcher[2];
motor_pid_t pid_poke;
motor_pid_t pid_rotation;

void abs_limit(float *candidate, float ABS_MAX) {
    if(*candidate > ABS_MAX){
        *candidate = ABS_MAX;
        return;
    }
    if(*candidate < -ABS_MAX){
        *candidate = -ABS_MAX;
    }
    return;
}

static void pid_reset(motor_pid_t *pid, float kp, float ki, float kd, float kv) {
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    pid->v = kv;
}

int get_circular_buffer_ind(int cur_idx, int enum_flag){
    return (cur_idx + HISTORY_DATA_SIZE - enum_flag) % HISTORY_DATA_SIZE;
}

int motor_val_translate(float _input){
    return _input;
}

int pid_calc(motor_pid_t *my_motor_pid, float desired_value){
    my_motor_pid->buffer_idx = (++my_motor_pid->buffer_idx) % HISTORY_DATA_SIZE;
    my_motor_pid->set[my_motor_pid->buffer_idx] = motor_val_translate(desired_value);
    my_motor_pid->err[my_motor_pid->buffer_idx] = 1;//TODO

    int LAST_POS = get_circular_buffer_ind(my_motor_pid->buffer_idx, LAST);
    int LLAST_POS = get_circular_buffer_ind(my_motor_pid->buffer_idx, LLAST);

    switch(my_motor_pid->pid_mode) {
        // Basic position PID; Used for chassis motor, gimbal auto-shooting mode
        case POSITION_PID:
        case GIMBAL_AUTO_SHOOT:
        case GIMBAL_MOUSE_IMU_SHOOT:
        case GIMBAL_MAN_SHOOT: //manual shoot
        case CHASSISS_ROTATE: // use chassis motor input to calibrate motor speed
            { //delete this bracket may cause compiler error
                // modified
                if (my_motor_pid->max_err != 0 && fabsf(my_motor_pid->err[NOW]) > my_motor_pid->max_err){
                    return 0;
                } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
                if (my_motor_pid->deadband != 0 && fabsf(my_motor_pid->err[NOW]) < my_motor_pid->deadband){
                    return 0;
                } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output

                if(my_motor_pid->pid_mode != CHASSISS_ROTATE){
                    if (my_motor_pid->err[NOW] >= 8191/2)
                        my_motor_pid->err[NOW] = my_motor_pid->err[NOW] - 8191;	//expected - measured
                    else if (my_motor_pid->err[NOW] <= -8191/2)
                        my_motor_pid->err[NOW] = my_motor_pid->err[NOW] + 8191;	//expected - measured
                }

                if(my_motor_pid->pid_mode == GIMBAL_MOUSE_IMU_SHOOT){
                    //do nothing for now
                }

                my_motor_pid->pout =  my_motor_pid->p *  my_motor_pid->err[NOW]; //pout=kp*err
                my_motor_pid->iout += my_motor_pid->i *  my_motor_pid->err[NOW]; //iout=sum[ki*err]
                my_motor_pid->dout =  my_motor_pid->d * (my_motor_pid->err[NOW] - my_motor_pid->err[LAST_POS]); //dout=kd*d(err)/dt
                my_motor_pid->vout =  my_motor_pid->v * get_motor_d_angle(my_motor_pid->motor); //vout=kv*v_get

                abs_limit(&(my_motor_pid->iout), my_motor_pid->IntegralLimit);
                my_motor_pid->pos_out = my_motor_pid->pout
                                        + my_motor_pid->iout + my_motor_pid->dout - my_motor_pid->vout;

                abs_limit(&(my_motor_pid->pos_out), my_motor_pid->MaxOutput);
                my_motor_pid->last_pos_out = my_motor_pid->pos_out; //Update last time
                break;
            }
        case DELTA_PID:
            if (my_motor_pid->err[NOW] >= 8191/2)
                my_motor_pid->err[NOW] = my_motor_pid->err[NOW] - 8191;	//expected - measured
            else if (my_motor_pid->err[NOW] <= -8191/2)
                my_motor_pid->err[NOW] = my_motor_pid->err[NOW] + 8191;	//expected - measured

            my_motor_pid->pout = my_motor_pid->p
                            * (my_motor_pid->err[NOW] - my_motor_pid->err[LAST_POS]); //pout=kp*d(err)/dt
            my_motor_pid->iout = my_motor_pid->i * my_motor_pid->err[NOW]; //iout=ki*err
            my_motor_pid->dout = my_motor_pid->d
                            * (my_motor_pid->err[NOW] - 2*my_motor_pid->err[LAST_POS] + my_motor_pid->err[LLAST_POS]); //dout=kd*d(d(err)/dt)/dt
            //my_motor_pid->vout = my_motor_pid->v
            //                * (my_motor_pid->get[NOW] - 2*my_motor_pid->get[LAST_POS] + my_motor_pid->get[LLAST_POS]); //vout=kv*a_get
            my_motor_pid->vout = 0;

            abs_limit(&(my_motor_pid->iout), my_motor_pid->IntegralLimit);

            my_motor_pid->delta_u = my_motor_pid->pout
                            + my_motor_pid->iout + my_motor_pid->dout - my_motor_pid->vout; //delta_u=pout+iout+dout-vout
            my_motor_pid->delta_out = my_motor_pid->last_delta_out
                            + my_motor_pid->delta_u; //delta_out=last_delta_out+delta_u
            abs_limit(&(my_motor_pid->delta_out), my_motor_pid->MaxOutput);
            my_motor_pid->last_delta_out = my_motor_pid->delta_out;

            // modified
            if (my_motor_pid->max_err != 0 && fabsf(my_motor_pid->err[NOW]) > my_motor_pid->max_err){
                my_motor_pid->delta_out = 0;
            } // PID does not apply if max_err=0 or err is larger than max_err; Keep original PID output
            if (my_motor_pid->deadband != 0 && fabsf(my_motor_pid->err[NOW]) < my_motor_pid->deadband){
                my_motor_pid->delta_out = 0;
            } // PID does not apply if deadband=0 or err is smaller than deadband; Keep original PID output
            break;
        case POKE:
            my_motor_pid->pout = my_motor_pid->p * my_motor_pid->err[NOW]; //pout=kp*err
            my_motor_pid->iout += my_motor_pid->i * my_motor_pid->err[NOW]; //iout=sum[ki*err]
            my_motor_pid->dout = my_motor_pid->d * (my_motor_pid->err[NOW] - my_motor_pid->err[LAST_POS]); //dout=kd*d(err)/dt
            my_motor_pid->vout = my_motor_pid->v * get_motor_d_angle(my_motor_pid->motor); //vout=kv*v_get
            abs_limit(&(my_motor_pid->iout), my_motor_pid->IntegralLimit);
            my_motor_pid->pos_out = my_motor_pid->pout + my_motor_pid->iout + my_motor_pid->dout - my_motor_pid->vout; //pos_out=pout+iout+dout-vout
            abs_limit(&(my_motor_pid->pos_out), my_motor_pid->MaxOutput);
            my_motor_pid->last_pos_out = my_motor_pid->pos_out; //Update last time
            break;
    }
    return 1;
}

void PID_struct_init(motor_pid_t *pid, uint32_t mode, uint32_t maxout,
        uint32_t intergral_limit, float _p, float _i, float _d, float _v) {
    /*Initialize pid param */
    pid->IntegralLimit = intergral_limit;
    pid->MaxOutput = maxout;
    pid->pid_mode = mode;

    pid->set = (uint16_t *)malloc(HISTORY_DATA_SIZE * sizeof(uint16_t));
    pid->err = (uint16_t *)malloc(HISTORY_DATA_SIZE * sizeof(uint16_t));
    //initialize the dynamic array to prevent garbage value
    for(int i = 0; i < HISTORY_DATA_SIZE; i++){
        pid->set[i] = 0;
        pid->err[i] = 0;
    }

    pid->buffer_idx = 0;

    pid->p = _p;
    pid->i = _i;
    pid->d = _d;
    pid->v = _v;

    /*set the default value for deadband and max_err*/
    pid->deadband = 1.0f; // set PID output tolerance
    pid->max_err = maxout; // set error upper bound
}
