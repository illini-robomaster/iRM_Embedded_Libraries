#include <key.h>
#include "taskGimbal.h"

extern osThreadId launcherTaskHandle;
/******************************************************************************
	Input:
	Output
	Description
		gimbal task
        this task is called by a timer for  20 ms period
        and set signal for launcherTask
	Log
		2018/04/08 Ke Sun
*******************************************************************************/
void StartGimbalTask(void const * argument){

    //RM_PRINTF("Start Gimbal Task; \r\n");
    // for GPIO
    uint8_t TARGET;
    uint8_t true_target=4;

        // update key-press
        static int key_sta, key_cnt;
        switch (key_sta) {
            case 0:    //no key
                if (RM_KEY_Pressed) { // active low for key pressing
                    key_sta = 1;    // if KEY is pressed set next state to case 1
                }
                break;
            case 1: //key down wait release.
                if (RM_KEY_Pressed) { // active low for key pressing
                    key_sta = 2;    // if KEY is pressed set next state to case 2
                    key_cnt++;      // if KEY is pressed update counter
                    RM_LED_RedOn();
                } else {
                    key_sta = 0;    // if KEY press is caused by noise, set next state to case 0
                }
                break;
            case 2:
                if (RM_KEY_Released) {
                    key_sta = 0;    // if KEY press is caused by noise, set next state to case 0
                    RM_LED_RedOff();
                }
                break;
        }
        if (key_cnt > 9) key_cnt = 0;


        /*	MOTOR RX MESSAGE
            For 3508,
                DATA[0]DATA[1] = Mechanical Angle in range 0 ~ 8191 - 0 ~ 360 degree
                DATA[2]DATA[3] = Rotational Speed in rpm
                DATA[4]DATA[5] = Actual torque current
                DATA[6] 	   = Motor temperature in Celcius
                DATA[7]		   = NULL
            For 6623,
                DATA[0]DATA[1] = Mechanical Angle in range 0 ~ 8191 - 0 ~ 360 degree
                DATA[2]DATA[3] = Actual torque current in range -13000 ~ 13000
                DATA[4]DATA[5] = Given torque current
                DATA[6]DATA[7] = NULL
        */
        // update chasis motor and gimbal motor according to the key-press

        // update chasis motor and gimbal motor according to the key-press


        // when remoteData.rc.s1 change to 1 set mode to auto-shooting
        if (remoteData.rc.s1 == 0) {
            GimbalPIDSetMode(GIMBAL_AUTO_SHOOT);

            //RM_GPIO_WriteBin(key_cnt);
            TARGET = RM_GPIO_ReadBin();
            if (TARGET != 14){
                true_target=TARGET;
            }
            //GimbalPIDAutoSet(true_target);

            GimbalPIDAutoSet(key_cnt);
        }
        // when remoteData.rc.s1 change to 0 set mode to mouse imu control on gimbal
//        else if (remoteData.rc.s1 == 0) {
//            GimbalPIDSetMode(GIMBAL_MOUSE_IMU_SHOOT);
//            GimbalPIDMouseIMU(remoteData.mouse.x*20, -30*remoteData.mouse.y);
//        }

        // when remoteData.rc.s1 change to 0 set mode to man-shooting with mouse
        else if (remoteData.rc.s1 == 1) {
            GimbalPIDSetMode(GIMBAL_MAN_SHOOT);
            GimbalPIDManSet(remoteData.mouse.x*27, -35*remoteData.mouse.y);
        }
            // when remoteData.rc.s1 change to 0 set mode to man-shooting with remote controllor
//        else if (remoteData.rc.s1 == 0) {
//            GimbalPIDSetMode(GIMBAL_MAN_SHOOT);
  //          GimbalPIDManSet(remoteData.rc.ch0-1024, remoteData.rc.ch1-1024);
//        }
         osSignalSet(launcherTaskHandle, LAUNCHER_TASK_EXE_SIGNAL);
		// RM_PRINTF("(%u, %u)pitch %d;yaw %d \r\n", motorPitch.angle, motorYaw.angle, (int) pid_pit.pos_out, (int) pid_yaw.pos_out);


}
