#ifndef _IMU_TASK_H_
#define _IMU_TASK_H_

#include "cmsis_os.h"

/* Global variable */
extern osThreadId   imu_task_handle;

/**
 * Create IMU task. Called in RTOS
 * @brief
 */
void imu_task_create(void);

/**
 * Irrelevant to RTOS. Just served as helper function to initialize IMU(s).
 * @brief
 * @return  1 means success. 0 means failure.
 */
uint8_t imu_task_init(void);

/**
 * RTOS thread. Periodically update IMU angles
 * @brief
 * @param argu RTOS arguments
 */
void imu_task(void const *argu);

#endif
