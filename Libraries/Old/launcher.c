//
// Created by Eric on 1/20/2018.
//

#include "launcher.h"

static int launcherRpm;
static int pokeAngle;
static bool launcherReady;

int NUMHOLES = 8;	//number of balls that the wheel can hold
int GEARRATIO = 19;	//Gear ratio of M3508-P19
int MAX_LAUNCHER_ERROR = 1000;	//Maximum error tolerance for shooting
int MAX_POKE_ERROR_ANGLE = 2; //Maximum error tolerance for shooting
int MAX_CURRENT_POKE = 4000; //The max current (0-16384) that can be proviced to the motor, increase from 4000, 8000, 12000, 16384 to test


void LauncherPIDSetMode(bool clockwise)	//Init poke direction
{
    clockwiseflag = clockwise;
    for (int i = 0; i < 2; i++) {
        PID_struct_init(&pid_launcher[i], POSITION_PID, 10000/2, 4000, 1.0f, 0.4f, 0.4f, 0.0f); //2 motors angular rate closeloop.
    }
    PID_struct_init(&pid_poke,POKE, MAX_CURRENT_POKE, 4000, 1.4f, 0.3f, 3.6f, 0.1f);	//1 motor position closeloop
    return ;
}
void LauncherPIDSet(int rpm) {	//Set the RPM of launcher wheels while holding poke
    launcherRpm = rpm;
    launcherReady = LauncherReady();
    pid_calc(&pid_launcher[0], motorLauncher[0].speedRPM, -rpm);
		pid_calc(&pid_launcher[1], motorLauncher[1].speedRPM, rpm);
    pid_calc(&pid_poke, motorPoke.totalAngle, pokeAngle);
    RM_CAN_SetMotor(&hcan1, CAN_3508_5_8_TX, (int16_t)(pid_launcher[0].pos_out), (int16_t) (pid_launcher[1].pos_out), (int16_t) (pid_poke.pos_out), 0);
    return ;
}

void LauncherPIDsetwithoutswitch(int rpm) {
  	int dAngle;
  	dAngle = pokeAngle - motorPoke.totalAngle;
  	if(dAngle > (GEARRATIO*8192)/NUMHOLES/2){
  		LauncherPIDSet(rpm);
  	}
  	else{
  		LauncherPIDSet(0);
  	}
}

bool LauncherShoot(void)	//Shoots, returns true if success
{
    if(LauncherReady()){
      if (clockwiseflag)
      {
          pokeAngle += (GEARRATIO*8192)/NUMHOLES;	//Spin the wheel
      }
      else
      {
         pokeAngle -=  (GEARRATIO*8192)/NUMHOLES;	//Spin the wheel
      }
      return true;
    }
    return false;
}
bool LauncherReady(void)	//Returns if the launcher is ready or not
{
    int dAngle;
    dAngle = pokeAngle - motorPoke.totalAngle;
    for (int i = 0; i < 2; i++) {
        if(abs(motorLauncher[i].speedRPM-launcherRpm) > MAX_LAUNCHER_ERROR )
        {
            return false;
        }
    }
    if(abs(dAngle) > (GEARRATIO*8192)/360*MAX_POKE_ERROR_ANGLE)
    {
        return false;
    }
    return true;
}
