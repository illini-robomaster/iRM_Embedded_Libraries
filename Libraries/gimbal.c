#include "gimbal.h"
#include "utils.h"

void gimbal_init(gimbal_t *my_gimbal) {
    /* Init Yaw */
    my_gimbal->yaw_middle  = MEASURED_MIDDLE_YAW;
    my_gimbal->yaw_ang = 0;
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    motor_t *yaw = motor_init(NULL, 0x209, CAN1_ID, M6623);
    my_gimbal->yaw = pid_init(NULL, ABSOLUTE_YAW, yaw, 0, 0, 2000, 500, 200, 4, 0.1, 20, 1600, 5, 0);
#elif defined(ENGINEERING)
    motor_t *yaw = motor_init(NULL, 0x209, CAN1_ID, M6623);
    pid_init(my_gimbal->yaw, GIMBAL_MAN_SHOOT, yaw, 3000, 6800, 1000000, 200, 200, 30, 0.02, 140, 3500, 5, 0);
#elif defined(HERO)
    morot_t *yaw = motor_init(NULL, 0x209, CAN1_ID, M6623);
    pid_init(my_gimbal->yaw, GIMBAL_MAN_SHOOT, yaw, 5200, 6800, 2000, 500, 200, 9, 0.1, 70, 1600, 5, 0);
#endif
    /* Init Pitch*/
    /* Init Camera Pitch */
    // not implemented yet
}

void gimbal_mouse_move(gimbal_t *my_gimbal, dbus_t *rc, int16_t observed_abs_yaw) {
    my_gimbal->yaw_ang -= rc->mouse.x * 0.2;
    //my_gimbal->pitch_ang -= rc->mouse.y * 0.2;
    my_gimbal->yaw->motor->out = pid_calc(my_gimbal->yaw, my_gimbal->yaw_ang - observed_abs_yaw);
    if (abs(my_gimbal->yaw->err[my_gimbal->yaw->idx]) > 100)
        my_gimbal->yaw->motor->out += sign(my_gimbal->yaw->err[my_gimbal->yaw->idx]) * 600; //700
    //my_gimbal->pitch->motor->out = pid_calc(&pid_pitch, my_gimbal->pitch_ang);
    // stop it if it reaches the true mech limit
}

void gimbal_update(gimbal_t *my_gimbal) {
    //get_motor_data(my_gimbal->pitch->motor);
    get_motor_data(my_gimbal->yaw->motor);
    //get_motor_data(my_gimbal->camera_pitch->motor);
}

void run_gimbal(gimbal_t *my_gimbal) {
    //set_motor_output(my_gimbal->pitch->motor, NULL, NULL, NULL);
    set_motor_output(my_gimbal->yaw->motor, NULL, NULL, NULL);
}
