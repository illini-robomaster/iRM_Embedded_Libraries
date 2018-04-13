#include "taskLauncher.h"

int set_launcher = 469*19; // goal velocity for launcher

void StartLauncherTask(void const * argument){
    osEvent event;
    while (1){
        RM_PRINTF("launcher task\n");
        event = osSignalWait(LAUNCHER_TASK_EXE_SIGNAL, osWaitForever);

        if(event.status == osEventSignal){
            if(event.value.signals & LAUNCHER_TASK_EXE_SIGNAL){
							  //RM_PRINTF("Start Launcher Task; \r\n");

                if(RM_Left_Clicked){
                    LauncherShoot();    //Shoot when button is pressed
                    // Launcher Spinning Wheel Speed (RPM)
                }
                LauncherPIDSet(set_launcher);
            }
        }
    }
}
