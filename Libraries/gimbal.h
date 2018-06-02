#ifndef _GIMBAL_H_
#define _GIMBAL_H_

#include "pid.h"
#include "motor.h"
#include "dbus.h"

#if defined(INFANTRY1)
    //#define MEASURED_LEFTMOST_YAW  7387
    #define MEASURED_MIDDLE_YAW    5990 // these data are measured on infantry1.
    #define MEASURED_MIDDLE_PITCH  6000
    //#define MEASURED_RIGHTMOST_YAW 4569
#elif defined(INFANTRY2)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#elif defined(INFANTRY3)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#elif defined(ENGINEERING)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#elif defined(HERO)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#endif

typedef struct {
    float pitch_ang;            // pitch err intergrated based on mouse input
    float yaw_ang;              // yaw error integrated from mouse movement
    pid_ctl_t *pitch;           // pitch motor pid
    int16_t yaw_middle;         // a pre-determined value for yaw motor
    pid_ctl_t *yaw;             // yaw motor pid
    pid_ctl_t *camera_pitch;    // camera pitch motor pid
} gimbal_t;

/**
 * Get feedbacks of motors related to gimbal
 * @brief
 * @param my_gimbal the desired gimbal struct; represents gimbal
 */
void gimbal_update(gimbal_t *my_gimbal);

/**
 * Initialize gimbal motors
 * @TODO: complete pid for pitch/camera_pitch
 * @brief
 * @param my_gimbal [description]
 */
void gimbal_init(gimbal_t *my_gimbal);

/**
 * calculate gimbal movement based on mouse feedback from mouse
 * @brief
 * @param my_gimbal        my gimbal object
 * @param rc               public dbus struct that holds mouse data
 * @param observed_abs_yaw absolute yaw angle with respect to the ground (instead of chassis)
 */
void gimbal_mouse_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw);

/**
 * calculate gimbal movement based on remote controller
 * @brief
 * @param my_gimbal        my gimbal object
 * @param rc               public dbus struct that holds remote controller data
 * @param observed_abs_yaw absolute yaw angle with respect to the ground (instead of chassis)
 */
void gimbal_remote_move(gimbal_t *my_gimbal, dbus_t *rc, int32_t observed_abs_yaw);

/**
 * Run motor with corresponding values. SHOULD ONLY BE CALLED AFTER PID CALC
 * @brief
 * @param my_gimbal my gimbal object
 */
void run_gimbal(gimbal_t *my_gimbal);

#endif
