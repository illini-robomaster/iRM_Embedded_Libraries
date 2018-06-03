#include "gimbal.h"
#include "utils.h"

void gimbal_init(gimbal_t *my_gimbal) {
    /* Init Yaw */
    motor_t *yaw;
    my_gimbal->yaw_middle  = MEASURED_MIDDLE_YAW;
    my_gimbal->yaw_ang = 0;
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    yaw = motor_init(NULL, 0x209, CAN1_ID, M6623);
    my_gimbal->yaw = pid_init(NULL, MANUAL_ERR_INPUT, yaw, 5200, 6800, 0, 0, 0, 20, 0, 70, 4500, 0);
#elif defined(ENGINEERING)
    // TODO
#elif defined(HERO)
    // TODO
#endif
    /* Init Pitch*/
    motor_t *pitch;
    my_gimbal->pitch_ang = MEASURED_MIDDLE_PITCH;
#ifdef ENGINEERING
    // TODO
#elif defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    pitch = motor_init(NULL, 0x20A, CAN1_ID, M6623);
    my_gimbal->pitch = pid_init(NULL, GIMBAL_MAN_SHOOT, pitch, 4800, 6200, 0, 0, 800, 4, 0.03, 15, 1800, 0);
#elif defined(HERO)
    // TODO
#endif
    /* Init Camera Pitch */
    // not implemented yet
}

void gimbal_mouse_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw) {
    my_gimbal->yaw_ang -= rc->mouse.x * 0.2;
    my_gimbal->pitch_ang -= rc->mouse.y * 0.2;
    my_gimbal->yaw->motor->out = pid_calc(my_gimbal->yaw, (int32_t)(my_gimbal->yaw_ang) - observed_abs_yaw);
    my_gimbal->pitch->motor->out = pid_calc(my_gimbal->pitch, (int32_t)(my_gimbal->pitch_ang));
    // stop it if it reaches the true mech limit
}

void gimbal_remote_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw) {
    my_gimbal->yaw_ang -= rc->ch2 * 0.1;
    my_gimbal->pitch_ang += rc->ch3 * 0.1;
    my_gimbal->yaw->motor->out = pid_calc(my_gimbal->yaw, (int32_t)my_gimbal->yaw_ang - observed_abs_yaw);
    my_gimbal->pitch->motor->out = pid_calc(my_gimbal->pitch, my_gimbal->pitch_ang);
}

void gimbal_update(gimbal_t *my_gimbal) {
    get_motor_data(my_gimbal->pitch->motor);
    get_motor_data(my_gimbal->yaw->motor);
    //get_motor_data(my_gimbal->camera_pitch->motor);
}

void run_gimbal(gimbal_t *my_gimbal) {
    /* Run Pitch */
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3) || defined(HERO)
    set_motor_output(my_gimbal->yaw->motor, my_gimbal->pitch->motor, NULL, NULL);
#else
    set_motor_output(my_gimbal->yaw->motor, NULL, NULL, NULL);
    set_motor_output(my_gimbal->pitch->motor, NULL, NULL, NULL);
#endif
    //set_motor_output(my_gimbal->camera_pitch->motor, NULL, NULL, NULL);
}
