#ifndef _GIMBAL_TASK_H_
#define _GIMBAL_TASK_H_

#include "cmsis_os.h"

#include "dbus.h"
#include "gimbal.h"
#define gimbal_period 5
/* Global variable */
extern osTimerId gimbal_timer_id;

void gimbal_task_create(void);
uint8_t gimbal_task_init(void);
void gimbal_task(void const *argu);

#endif
