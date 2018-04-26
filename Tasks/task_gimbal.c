#include "task_gimbal.h"

osTimerId gimbal_timer_id;

void gimbal_task_create(void){
    print("gimbal Function called\r\n");

    osTimerDef(gimbal_timer, gimbal_task); // create gimbal timer
    gimbal_timer_id = osTimerCreate(osTimer(gimbal_timer), osTimerPeriodic,NULL);
    if(gimbal_timer_id == NULL){
        print("gimbal timer create failed.\r\n");
    }else{
        print("gimbal timer created.\r\n");
    }
}

uint8_t gimbal_task_init(void){
    print("gimbal init");
    return 0;
}
void gimbal_task(void const *argu){
    print("gimbal task start");
}
