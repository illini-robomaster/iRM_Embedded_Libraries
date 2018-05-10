#ifndef _GIMBAL_H_
#define _GIMBAL_H_

#include "pid.h"
#include "motor.h"
#include "dbus.h"

typedef enum{
    YAW_LEFTMOST  = 0,
    YAW_MIDDLE = 1,
    YAW_RIGHTMOST  = 2
} gimbal_yaw_measurement_t;

#if defined(INFANTRY1)
    #define MEASURED_LEFTMOST_YAW  7387
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_RIGHTMOST_YAW 4569
#elif defined(INFANTRY2)
    #define MEASURED_LEFTMOST_YAW  7387
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_RIGHTMOST_YAW 4569
#elif defined(INFANTRY3)
    #define MEASURED_LEFTMOST_YAW  7387
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_RIGHTMOST_YAW 4569
#elif defined(ENGINEERING)
    #define MEASURED_LEFTMOST_YAW  7387
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_RIGHTMOST_YAW 4569
#elif defined(HERO)
    #define MEASURED_LEFTMOST_YAW  7387
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_RIGHTMOST_YAW 4569
#endif

typedef struct {
    pid_ctl_t *pitch;
    int16_t yaw_range[3];
    pid_ctl_t *yaw;
    pid_ctl_t *camera_pitch;
} gimbal_t;

void gimbal_init(gimbal_t *my_gimbal);

void gimbal_mouse_move(gimbal_t *my_gimbal, dbus_t *rc);

#endif
