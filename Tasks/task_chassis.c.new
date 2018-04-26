#include "task_chassis.h"

//static osEvent chassis_event;
osTimerId chassis_timer_id;

void chassis_task_create(void){
    print("Function called\r\n");
    osTimerDef(ChassisTimer, chassis_task); // create chassis timer
    chassis_timer_id = osTimerCreate(osTimer(ChassisTimer), osTimerPeriodic, NULL);

    if (chassis_timer_id == NULL)
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
    chassis_t my_chassis;
    print("SANITY 1\r\n");
    chassis_task_init(&my_chassis);
    print("SANITY 2\r\n");
    dbus_t* rc = dbus_get_struct();
    print("SANITY 3\r\n");
    uint32_t chassis_wake_time = osKernelSysTick();
    print("SANITY 4\r\n");

    print("CHASSIS LOOP SENDS GREETS\r\n");
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
    free(my_chassis.m_fl);
    free(my_chassis.m_fr);
    free(my_chassis.m_rl);
    free(my_chassis.m_rr);
    print("CHASSIS ELAPSED: %dSANITY\r\n", HAL_GetTick() - tickStart);
}
