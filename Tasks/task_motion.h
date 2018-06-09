#ifndef _MOTION_TASK_H_
#define _MOTION_TASK_H_

#include "cmsis_os.h"

/* Start User Include */
#include "dbus.h"
#include "motor.h"
#include "chassis.h"
#include "gimbal.h"
#include "imu_onboard.h"
/* End User Include */

#define MOTION_CYCLE 20
#define MOTOR_2_RAD  0.0007669904f // (360 / 8192) * (pi / 180)
#define DEG_2_MOTOR  22.75556f // (8192 / 360)

/* Global variable */
extern osThreadId chassis_task_handle;
extern osThreadId gimbal_task_handle;

extern gimbal_t my_gimbal;
extern pid_ctl_t* my_chassis[4];

typedef enum {
    NORMAL,
    REVERSE,
}   motion_mode_t;

/**
 * get current yaw angle for gimbal
 * @brief
 * @param gimbal_motor ptr for gimbal motor. We need feedback from it.
 * @return  current yaw angle for gimbal in degree (without wrap around. E.g., you may get 370)
 */
float get_gimbal_yaw_angle(motor_t *gimbal_motor);

/**
 * RTOS helper function to create thread for motion task
 * @brief
 */
void motion_task_create(void);

/**
 * RTOS thread function. SHOULD NEVER RETURN.
 * @brief
 * @param argu RTOS arguments
 */
void chassis_task(void const *argu);

/**
 * RTOS thread function. SHOULD NEVER RETURN.
 * @brief
 * @param argu RTOS arguments
 */
void gimbal_task(void const *argu);

#endif
