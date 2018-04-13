#include "taskIMU.h"
/******************************************************************************
	Input:
	Output
	Description
		IMU task, 20 ms
	Log
		2018/04/08 Ke Sun
*******************************************************************************/
void StartIMUTask(void const * argument) {
    uint32_t imu_wake_time = osKernelSysTick();
    while(1){
        //RM_PRINTF("Start IMU Task; \r\n");
        // print onboard imu
        // RM_DEBUG_IMU_PrintRaw();
        // RM_DEBUG_IMU_PrintDecoded();

        RM_MPU6500_ANGLE_UPDATE(); //update angle

        // print imu
        //RM_DEBUG_IMU_PrintKalman();
        // print raw magnetometer data
        //RM_DEBUG_IMU_PrintRaw();
        //RaiseRequest();
        //HAL_Delay(1000);

        osDelayUntil(&imu_wake_time, 20);
    }
}
