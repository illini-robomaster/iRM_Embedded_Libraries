#include "gimbal.h"
#include "imu_onboard.h"
#include "motor.h"
#include "utils.h"

void gimbal_init(gimbal_t *my_gimbal) {
    /* Init Yaw */
    motor_t *yaw;
    my_gimbal->yaw_middle   = INIT_MIDDLE_YAW;
    my_gimbal->yaw_ang      = 0;
#if defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    yaw = motor_init(NULL, 0x209, CAN1_ID, M6623);
    my_gimbal->yaw = pid_init(NULL, MANUAL_ERR_INPUT, yaw, 5200, 6800, 0, 0, 0, 20, 0, 70, 4500, 0);
#elif defined(ENGINEERING)
    yaw = motor_init(NULL, 0x209, CAN1_ID, M6623);
    my_gimbal->yaw = pid_init(NULL, MANUAL_ERR_INPUT, yaw, 0, 0, 0, 0, 0, 4.2, 0, 35, 2500, 0);
#elif defined(HERO)
    // TODO
#endif
    /* Init Pitch*/
    motor_t *pitch;
    my_gimbal->pitch_ang = INIT_MIDDLE_PITCH;
#ifdef ENGINEERING
    pitch = motor_init(NULL, 0x205, CAN1_ID, M3510);
    my_gimbal->pitch = pid_init(NULL, GIMBAL_MAN_SHOOT, pitch, PITCH_LOW_LIMIT, PITCH_HIGH_LIMIT, 0, 0, 0, 4, 0, 8, 8000, 0);
#elif defined(INFANTRY1) || defined(INFANTRY2) || defined(INFANTRY3)
    pitch = motor_init(NULL, 0x20A, CAN1_ID, M6623);
    my_gimbal->pitch = pid_init(NULL, GIMBAL_MAN_SHOOT, pitch, PITCH_LOW_LIMIT, PITCH_HIGH_LIMIT, 0, 0, 800, 4, 0.03, 15, 1800, 0);
#elif defined(HERO)
    // TODO
#endif
    /* Init Camera Pitch */
    // not implemented yet
}

void gimbal_mouse_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw) {
    my_gimbal->yaw_ang -= rc->mouse.x * 0.2;
    //TODO: pitch_ang needs to be clipped to range
    my_gimbal->pitch_ang -= rc->mouse.y * 0.2;
    my_gimbal->yaw->motor->out = pid_calc(my_gimbal->yaw, (int32_t)(my_gimbal->yaw_ang) - observed_abs_yaw);
    my_gimbal->pitch->motor->out = pid_calc(my_gimbal->pitch, (int32_t)(my_gimbal->pitch_ang));
}

void gimbal_remote_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw) {
    my_gimbal->yaw_ang -= rc->ch2 * 0.1;
    //TODO: pitch_ang needs to be clipped to range
    my_gimbal->pitch_ang += rc->ch3 * 0.1;
    my_gimbal->yaw->motor->out = pid_calc(my_gimbal->yaw, (int32_t)my_gimbal->yaw_ang - observed_abs_yaw);
    my_gimbal->pitch->motor->out = pid_calc(my_gimbal->pitch, my_gimbal->pitch_ang);
}

void gimbal_update(gimbal_t *my_gimbal) {
    //get_motor_data(my_gimbal->pitch->motor);
    get_motor_data(my_gimbal->yaw->motor);
    //get_motor_data(my_gimbal->camera_pitch->motor);
}

void yaw_ramp_ctl(gimbal_t *my_gimbal, int32_t delta_ang, uint16_t step_size) {
    int8_t direction = sign(delta_ang);
    int32_t observed_abs_yaw;

    for (size_t i = 0; i < abs(delta_ang); i += step_size) {
        observed_abs_yaw = (int32_t)(imuBoard.angle[YAW] * DEG_2_MOTOR);
        my_gimbal->yaw_ang += direction * step_size;
        my_gimbal->yaw->motor->out = pid_calc(my_gimbal->yaw, (int32_t)(my_gimbal->yaw_ang) - observed_abs_yaw);
        my_gimbal->pitch->motor->out = pid_calc(my_gimbal->pitch, (int32_t)my_gimbal->pitch_ang);
        run_gimbal(my_gimbal);
        osDelay(20);
    }
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
