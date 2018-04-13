#include "AllDimensionalMotion.h"
#include "chassis.h"
/*
 *  _angle 0~359: clockwise
 *               0
 *         270       90
 *              180
*/
void RM_All_D_Motion(double _angle, bool go){
    int forward_back, left_right;

    forward_back = cos(_angle*PI/180.0)*2000;
    left_right = -sin(_angle*PI/180.0)*2000;

    int set[4] = {0, 0, 0, 0};
    if (go){
        //Mecanuum physics and linear algebra basis(Mechanical Engineering & MATH415)
        set[0] = forward_back*0.75 - left_right*0.75;
        set[1] = -forward_back*0.75 - left_right*0.75;
        set[2] = forward_back*0.75 + left_right*0.75;
        set[3] = -forward_back*0.75 + left_right*0.75;
    }

    // Motor PID calculation
    for (int i = 0; i < 4; i++) {
        pid_calc(&pid_spd[i], motorChassis[i].speedRPM, set[i]);
    }
    //RM_PRINTF("Start Chassis Task\r\n");
    // Motor Output PID
    RM_CAN_SetMotor(&hcan1, CAN_3508_1_4_TX, (int16_t)(pid_spd[0].pos_out), (int16_t)(pid_spd[1].pos_out), (int16_t)(pid_spd[2].pos_out), (int16_t)(pid_spd[3].pos_out));

}








/*#include "ChassisControlRemoter.h"
#include "dbus.h"
void Remote_Control(void){
double Haxis;
double Vaxis;
double Raxis;
Haxis = sqrt(abs(remoteData.rc.ch0 - 1024));
Vaxis = sqrt(abs(remoteData.rc.ch1 - 1024));
Haxis = ((Haxis > 1.5) ? Haxis : 0);
Vaxis = ((Vaxis > 1.5) ? Vaxis : 0);
// horizontal & vertical movement control
if (remoteData.rc.ch1-1024 >= 0 && remoteData.rc.ch0-1024 >= 0 && remoteData.rc.ch1 + remoteData.rc.ch0 - 2048 > 4.5) {
    // Speed set to 0 if initial input sum within[-2.5 2.5] to avoid unexpected movement at the beginning
    set_spd[1] = (-Haxis + Vaxis)* 45; // If multiplied by 1, ch1 and ch0 will cancel rather than linearly add up
    set_spd[3] = (Haxis + Vaxis)* 45;
    set_spd[0] = (-Haxis - Vaxis) * 45;
    set_spd[2] = (Haxis - Vaxis) * 45;
}else if (remoteData.rc.ch1-1024 < -0 && remoteData.rc.ch0-1024 >= 0 && -(remoteData.rc.ch1-1024) + remoteData.rc.ch0-1024 > 4.5){
    set_spd[1] = (-Haxis - Vaxis)* 45;
    set_spd[3] = (Haxis - Vaxis)* 45;
    set_spd[0] = (-Haxis + Vaxis)* 45;
    set_spd[2] = (Haxis + Vaxis)* 45;;
}else if (remoteData.rc.ch1-1024 >= 0 && remoteData.rc.ch0-1024 < -0 && remoteData.rc.ch1-1024 -(remoteData.rc.ch0-1024) > 4.5){
    set_spd[1] = (Haxis + Vaxis)* 45;
    set_spd[3] = (-Haxis + Vaxis)* 45;
    set_spd[0] = (Haxis - Vaxis)* 45;
    set_spd[2] = (-Haxis - Vaxis)* 45;
}else if (remoteData.rc.ch1-1024 < -0 && remoteData.rc.ch0-1024 < -0 && -(remoteData.rc.ch1-1024) -(remoteData.rc.ch0-1024) > 4.5){
    set_spd[1] = (Haxis - Vaxis)* 45;
    set_spd[3] = (-Haxis - Vaxis)* 45;
    set_spd[0] = (Haxis + Vaxis)* 45;
    set_spd[2] = (-Haxis + Vaxis)* 45;

// Spinning control
}else {
    set_spd[1] = 0;
    set_spd[3] = 0;
    set_spd[0] = 0;
    set_spd[2] = 0;
}
Raxis = sqrt(abs(-remoteData.rc.ch3 + 1024));
if (remoteData.rc.ch3-1024 < -1.5 ){
    // Speed will be set to 0 if initial input within [-1.5, 1.5] to avoid unexpected movement at the beginning
set_spd[1] += Raxis * 25;
    set_spd[3] += Raxis * 25;
    set_spd[0] += Raxis * 25;
    set_spd[2] += Raxis * 25;
}else if (remoteData.rc.ch3-1024 > 1.5 ){
set_spd[1] += -Raxis * 25;
    set_spd[3] += -Raxis * 25;
    set_spd[0] += -Raxis * 25;
    set_spd[2] += -Raxis * 25;
}
}
*/
