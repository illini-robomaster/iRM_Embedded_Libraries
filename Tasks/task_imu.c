#include "task_imu.h"
#include "imu_onboard.h"

static osEvent imu_event;
osThreadId imu_task_handle;

void imu_task_create(void){
    osThreadDef(imu_task_name, imu_task, osPriorityAboveNormal, 0, 512);
    imu_task_handle = osThreadCreate(osThread(imu_task_name), NULL);
#ifdef DEBUG
    BSP_DEBUG;
    if (imu_task_handle == NULL)
        print("IMU task create failed.\r\n");
    else
        print("IMU task created.\r\n");
#endif
}

uint8_t imu_task_init(void){
    print("IMU task initing\r\n");
    onboard_imu_lib_init();
    return 1; // no error handler right now
}

void imu_task(void const *argu){
    int idle_period = DT * 1000;
    print("IDLE PERIOD: %d", idle_period);
    uint32_t imu_wake_time = osKernelSysTick();
    while (1) {
        //print("IMU task loop sends greets :D .\r\n");
        uint32_t tickStart = HAL_GetTick();
        onboard_imu_update();
        print_imu_data();
        osDelayUntil(&imu_wake_time, idle_period);
    }
}
