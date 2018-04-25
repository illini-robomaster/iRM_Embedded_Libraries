#ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_

#include "cmsis_os.h"

#include "dbus.h"
#include "chassis.h"

/* Global variable */
extern osThreadId   chassis_task_handle;

void chassis_task_create(void);
uint8_t chassis_task_init(chassis_t *my_chassis);
void chassis_task(void const *argu);

#endif
