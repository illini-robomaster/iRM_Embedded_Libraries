#include "task_chassis.h"

//static osEvent chassis_event;
osThreadId chassis_task_handle;

void chassis_task_create(void){
    print("Function called\r\n");
    osThreadDef(chassis_task_name, chassis_task, osPriorityAboveNormal, 0, 128);
    chassis_task_handle = osThreadCreate(osThread(chassis_task_name), NULL);
    if (chassis_task_handle == NULL)
        print("chassis task create failed.\r\n");
    else
        print("chassis task created.\r\n");
}

uint8_t chassis_task_init(chassis_t *my_chassis){
    print("chassis task initing\r\n");
    chassis_init(my_chassis);
    return 1; // no error handler right now
}

void chassis_task(void const *argu){
    print("CHASSIS TASK STARTED\r\n");
    chassis_t *my_chassis = (chassis_t*)malloc(sizeof(chassis_t) * 4);
    chassis_task_init(my_chassis);
    dbus_t* rc = dbus_get_struct();
    while (1) {
        float yaw_angle = 0.5235988; // not implemented yet
        calc_keyboard_move(my_chassis, rc, yaw_angle);
        calc_gimbal_compensate(my_chassis, yaw_angle);
        run_chassis(my_chassis);
        osDelay(5);
    }
}
