#include "poker.h"
#include "pid.h"

void poker_init(poker_ctl_t *my_poker, float _kp, float _ki, motor_t *_motor){
    my_poker->kp = _kp;
    my_poker->ki = _ki;
    my_poker->motor = _motor;
}

void single_shot(poker_ctl_t *my_poker){
    pid_ctl_t brake_pid;
    pid_init(&brake_pid, POKE, my_poker->motor, -3000, 0, 1000, 0, 0, 44, 0, 50, 10000, 5, 400);

    uint8_t poked_flag = 0;
    int16_t lerr = 0;
    int16_t llerr = 0;
    int remaining_dist = AVG_ANGLE_SUM;

    get_motor_data(my_poker->motor);
    int16_t last_angle = get_motor_angle(my_poker->motor);

    while((poked_flag == 0) && (remaining_dist > 0)){
        get_motor_data(my_poker->motor);
        int16_t new_ang = get_motor_angle(my_poker->motor);
        remaining_dist -= clip_angle_err(my_poker->motor, new_ang - last_angle);
        last_angle = new_ang;

        int16_t err = get_speed_err(my_poker->motor, NORMAL_SPEED);
        //print("Err: %d", err);

        float pout = my_poker->kp * err;
        if(pout > 10000 || pout < -10000)
            pout = -1000;

        //print("Yooo: %d", remaining_dist);
        //print("lerr: %d", lerr);
        //print("llerr: %d", llerr);

        if(lerr + err + llerr > RESIST_THRES || lerr + err + llerr < -RESIST_THRES){
            poked_flag = 1;
            my_poker->motor->out = NUKER;
            set_motor_output(NULL, NULL, NULL, my_poker->motor);
            HAL_Delay(BURST_TIME);
        } else {
            my_poker->motor->out = pout;
            set_motor_output(NULL, NULL, NULL, my_poker->motor);
        }
        llerr = lerr;
        lerr = err;
        last_angle = get_motor_angle(my_poker->motor);
        HAL_Delay(5);
    }
    // brake!!
    //print("getting into poker......\r\n");
    get_motor_data(my_poker->motor);
    int16_t err = get_speed_err(my_poker->motor, 0);
    //print("New error: %d", err);
    while(abs(err) > 100){
        my_poker->motor->out = pid_speed_ctl_speed(&brake_pid, 0);
        //print("Brake Output: %d", my_poker->motor->out);
        set_motor_output(NULL, NULL, NULL, my_poker->motor);
        HAL_Delay(5);
        get_motor_data(my_poker->motor);
        err = get_speed_err(my_poker->motor, 0);
    }
    my_poker->motor->out = 0;
    set_motor_output(NULL, NULL, NULL, my_poker->motor);
    HAL_Delay(5);
}
