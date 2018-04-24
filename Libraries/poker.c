#include "poker.h"

void poker_init(poker_ctl_t *my_poker, float _kp, float _ki, motor_t *_motor){
    my_poker->kp = _kp;
    my_poker->ki = _ki;
    my_poker->motor = _motor;
}

void single_shot(poker_ctl_t *my_poker){
    pid_ctl_t brake_pid;
    pid_init(&brake_pid, POKE, my_poker->motor, -3000, 0, 1000, 0, 0, 44, 0, 50, 10000, 5, 400);

    uint8_t poked_flag = 0;
    float integrator = 0;
    int remaining_dist = AVG_ANGLE_SUM;

    get_motor_data(my_poker->motor);
    int16_t last_angle = get_motor_angle(my_poker->motor);

    while((poked_flag == 0) && (remaining_dist > 0)){
        int16_t new_ang = get_motor_angle(my_poker->motor);
        remaining_dist -= clip_angle_err(my_poker->motor, new_ang - last_angle);
        last_angle = new_ang;

        int16_t err = get_speed_err(my_poker->motor, NORMAL_SPEED);

        float pout = my_poker->kp * err;
        integrator += my_poker->ki * err;

        print("Int: %d", integrator);

        if(integrator > RESIST_THRES){
            poked_flag = 1;
            my_poker->motor->out = -10000;
            //set_motor_output(NULL, NULL, NULL, my_poker->motor);
            HAL_Delay(5);
        } else {
            my_poker->motor->out = pout;
            //set_motor_output(NULL, NULL, NULL, my_poker->motor);
        }

        print("Poker Output: %d", my_poker->motor->out);
        last_angle = get_motor_angle(my_poker->motor);
        HAL_Delay(5);
    }
    // brake!!
    int16_t err = get_speed_err(my_poker->motor, 0);
    while(abs(err) > 250){
        my_poker->motor->out = pid_speed_ctl_speed(&brake_pid, 0);
        print("Poker Output: %d", my_poker->motor->out);
        //set_motor_output(NULL, NULL, NULL, my_poker->motor);
        HAL_Delay(5);
        err = get_speed_err(my_poker->motor, 0);
    }
}
