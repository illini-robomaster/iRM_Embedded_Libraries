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

#define MOTION_CYCLE 5
#define MOTOR_2_RAD  0.0007669904 // (360 / 8192) * (pi / 180)
#define DEG_2_MOTOR  22.75556 // (8192 / 360)

/* Global variable */
extern osThreadId motion_task_handle;

/**
 * get current yaw angle for gimbal
 * @brief
 * @param gimbal_motor ptr for gimbal motor. We need feedback from it.
 * @return  current yaw angle for gimbal in degree (without wrap around. E.g., you may get 370)
 */
float get_gimbal_yaw_angle(motor_t *gimbal_motor);

/**
 * Helper function that initializes gimbal
 * @brief
 * @return  pointer to initialized gimbal
 */
gimbal_t *gimbal_task_init(void);

/**
 * Helper function that initializes chassis
 * @brief
 * @return  ptr to initialized chassis
 */
chassis_t *chassis_task_init(void);

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
void motion_task(void const *argu);

#endif
