#include "task_chassis.h"

static osEvent chassis_event;
osThreadId chassis_task_handle;

void chassis_task_create(void){
    osThreadDef(chassis_task_name, chassis_task, osPriorityNormal, 0, 256);
    chassis_task_handle = osThreadCreate(osThread(chassis_task_name), NULL);
#ifdef DEBUG
    BSP_DEBUG;
    if (chassis_task_handle == NULL)
        print("chassis task create failed.\r\n");
    else
        print("chassis task created.\r\n");
#endif
}

uint8_t chassis_task_init(chassis_t *my_chassis){
    print("chassis task initing\r\n");
    chassis_init(my_chassis);
    return 1; // no error handler right now
}

void chassis_task(void const *argu){
    chassis_t my_chassis;
    chassis_task_init(&my_chassis);
    dbus_t* rc = dbus_get_struct();
    uint32_t chassis_wake_time = osKernelSysTick();
    while (1) {
        uint32_t tickStart = HAL_GetTick();
        if (rc->key.bit.W) {
            calc_chassis_output(&my_chassis, 1, 0, 0.5);
        }
        else if (rc->key.bit.S) {
            calc_chassis_output(&my_chassis, -1, 0, 0.5);
        }
        else if (rc->key.bit.A) {
            my_chassis.pid_fl->motor->out = pid_calc(my_chassis.pid_fl, -MAX_SPEED);
            my_chassis.pid_fr->motor->out = pid_calc(my_chassis.pid_fr, -MAX_SPEED);
            my_chassis.pid_rl->motor->out = pid_calc(my_chassis.pid_rl, MAX_SPEED);
            my_chassis.pid_rr->motor->out = pid_calc(my_chassis.pid_rr, MAX_SPEED);
        }
        else if (rc->key.bit.D) {
            my_chassis.pid_fl->motor->out = pid_calc(my_chassis.pid_fl, MAX_SPEED);
            my_chassis.pid_fr->motor->out = pid_calc(my_chassis.pid_fr, MAX_SPEED);
            my_chassis.pid_rl->motor->out = pid_calc(my_chassis.pid_rl, -MAX_SPEED);
            my_chassis.pid_rr->motor->out = pid_calc(my_chassis.pid_rr, -MAX_SPEED);
        }
        else if (rc->key.bit.Q) {
            calc_chassis_output(&my_chassis, 1, 0, 0.5);
        }
        else if (rc->key.bit.E) {
            calc_chassis_output(&my_chassis, 1, 0, 0.5);
        }
        else {
            calc_chassis_output(&my_chassis, 0, 0, 0);
        }
        run_chassis(&my_chassis);
        osStatus ret_stat = osDelayUntil(&chassis_wake_time, calc_interval);
        if(ret_stat != osOK){
            print("DELAY WAS NOT EXECTUTED!!!!!!!!!!");
        }
        print("CHASSIS ELAPSED: %dSANITY\r\n", HAL_GetTick() - tickStart);
    }
}
