#ifndef _IMU_TASK_H_
#define _IMU_TASK_H_

#include "cmsis_os.h"

/* Global variable */
extern osThreadId   imu_task_handle;

void imu_task_create(void);
uint8_t imu_task_init(void);
void imu_task(void const *argu);

#endif
