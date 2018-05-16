#include "gimbal.h"

void gimbal_init(gimbal_t *my_gimbal) {
    /* Init Yaw */
    my_gimbal->yaw_middle  = MEASURED_MIDDLE_YAW;
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    motor_t *yaw = motor_init(NULL, 0x209, CAN1_ID, M6623);
    pid_init(my_gimbal->yaw, GIMBAL_MAN_SHOOT, yaw, 5200, 6800, 2000, 500, 200, 9, 0.1, 70, 1600, 5, 0);
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
    // nothing yet
    /*yaw_ang -= rc->mouse.x * 0.2;
    //pitch_ang -= rc->mouse.y * 0.2;
    m_yaw.out = pid_calc(&pid_yaw, yaw_ang);
    m_pitch.out = pid_calc(&pid_pitch, pitch_ang);
    // stop it if it reaches the true limit
    yaw_ang = clip_to_range(yaw_ang - rc->mouse.x * 0.2,
            pid_yaw.low_lim, pid_yaw.high_lim);
    pitch_ang = clip_to_range(pitch_ang - rc->mouse.y * 0.2,
            pid_pitch.low_lim, pid_pitch.high_lim);

    set_motor_output(&m_pitch, NULL, NULL, NULL);
    set_motor_output(&m_yaw, NULL, NULL, NULL);
    */
}

void gimbal_update(gimbal_t *my_gimbal) {
    get_motor_data(my_gimbal->pitch->motor);
    get_motor_data(my_gimbal->yaw->motor);
    get_motor_data(my_gimbal->camera_pitch->motor);
}

void run_gimbal(gimbal_t *my_gimbal) {

}
