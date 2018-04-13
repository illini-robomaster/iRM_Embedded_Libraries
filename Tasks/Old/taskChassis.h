#ifndef TASKCHASSIS__H
#define TASKCHASSIS__H

#include "mytype.h"
#include "chassis.h"
/* chassis control period (ms) */
#define CHASSIS_PERIOD 10
#define total_cur_limit 2 /*current value needs to be set*/
/******************************************************************************
    VARIABLES
*******************************************************************************/
static float curr_ang;
static float last_ang;
static float desired_ang;
static float ang_velocity;
static int set_v;
static int set_pit;      //goal angle for pitch
static int set_yaw;      // goal angle for yaw
static int rotation=0;
static bool rot=false;
static bool rrot=false;
static int forward_back_ref,left_right_ref,rotate_ref;// goal velocity for chassis
static double set[4];

/*  The function achieve remote control on the robot by using ch0, ch1, ch3(may consider switching to ch2)
    Outputs may be optimized by futher parametrization
    ch0 = horizontal stick on the right
    ch1 = vertical stick on the right
    ch3 = vertical stick on the left(up = clockwise, down = counterclockwise)
    chassis top view:
    set_spd[0]-----------set_spd[1]
                ---
                ---
                ---
    set_spd[2]-----------set_spd[3]
*/
void RM_Chassis_ControlInstructions(void);
void power_limit_control(double* speed);
void StartChassisTask(void const * argument);

#endif
/******************************************************************************
	Description
    keyboard Control instructions:
    when no key is pressed, chassis does nothing
    when W is pressed, chassis moves forward
    when S is pressed, chassis moves backward
    when A is pressed, chassis moves left
    when D is pressed, chassis moves right
    when Q is pressed, chassis moves counterclockwise
    when R is pressed, chassis moves clockwise
    when Z is pressed, chassis keeps going clockwise and counterclockwise in repeated cycles. Press X to stop this action.
    when F is pressed, chassis moves forward in high speed
    when V is pressed, chassis moves backward in high speed
    when C is pressed, chassis moves left with high speed
    when B is pressed, chassis moves right with high speed
  Log
    12/09/17 Ke Sun, Ruihao Yao First Draft
    12/20/17 Ruihao Yao Second Draft

*******************************************************************************/
