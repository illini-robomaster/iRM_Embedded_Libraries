#include "gimbal.h"

void gimbal_init(gimbal_t *my_gimbal) {
    /* Init Yaw */
    my_gimbal->yaw_range[YAW_LEFTMOST]  = MEASURED_LEFTMOST_YAW;
    my_gimbal->yaw_range[YAW_MIDDLE]    = MEASURED_MIDDLE_YAW;
    my_gimbal->yaw_range[YAW_RIGHTMOST] = MEASURED_RIGHTMOST_YAW;
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
}

void gimbal_mouse_move(gimbal_t *my_gimbal, dbus_t *rc) {
    // nothing yet
}
