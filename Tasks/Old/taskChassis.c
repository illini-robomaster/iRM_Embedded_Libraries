#include "taskChassis.h"
#include "mytype.h"
#include <math.h>



/******************************************************************************
	Input:
	Output
	Description
		chassis task
        this task is called by a timer for  10 ms period
	Log
		2018/04/08 Ke Sun & Steven Chen
*******************************************************************************/
void StartChassisTask(void const * argument){

    curr_ang = imuBoard.corrected_angle_data.z;
    desired_ang = curr_ang+PI/4-last_ang;
    ang_velocity = curr_ang-last_ang;
    forward_back_ref = left_right_ref=rotate_ref=0;

    // Mecanuum physics and linear algebra basis(Mechanical Engineering & MATH415)
    RM_Chassis_ControlInstructions();
    set[0] = (forward_back_ref*0.75 - left_right_ref*0.75 - 0.75*rotate_ref)*sin(desired_ang)-1.0*ang_velocity;
    set[1] = (-forward_back_ref*0.75 - left_right_ref*0.75 - 0.75*rotate_ref)*cos(desired_ang)+1.0*ang_velocity;
    set[2] = (forward_back_ref*0.75 + left_right_ref*0.75 - 0.75*rotate_ref)*cos(desired_ang)+1.0*ang_velocity;
    set[3] = (-forward_back_ref*0.75 + left_right_ref*0.75 - 0.75*rotate_ref)*sin(desired_ang)-2.0*ang_velocity;

	//power limit
	//power_limit_control(set);

    // Motor PID calculation
    for (int i = 0; i < 4; i++) {
        pid_calc(&pid_spd[i], motorChassis[i].speedRPM, set[i]);
    }
  	//RM_PRINTF("Start Chassis Task\r\n");

    RM_CAN_SetMotor(&hcan1, CAN_3508_1_4_TX, (int16_t)(pid_spd[0].pos_out), (int16_t)(pid_spd[1].pos_out), (int16_t)(pid_spd[2].pos_out), (int16_t)(pid_spd[3].pos_out));
    last_ang = curr_ang;
    ang_velocity=0;
}
/******************************************************************************
	Input:
	Output
	Description
		RM Chassis Control Instructions
	Log
		2018/04/08 Ke Sun
*******************************************************************************/
void RM_Chassis_ControlInstructions(void){
	   // init_ang=curr_ang=last_ang=imuBoard.corrected_angle_data.z;
		  //    Motor PID calculation
		  // forward motion when pressing W
			if(RM_W_Pressed){
				forward_back_ref += 2000;
				left_right_ref += 0;
				rotate_ref += 0;
			}
		  // backward motion when pressing S
			if(RM_S_Pressed){
				forward_back_ref += -2000;
				left_right_ref += 0;
				rotate_ref += 0;
			}
		  // left motion when pressing A
			if(RM_A_Pressed){
				forward_back_ref += 0;
				left_right_ref += 2000;
				rotate_ref += 0;
			}
		  // right motion when pressing D
			if(RM_D_Pressed){
				forward_back_ref += 0;
				left_right_ref += -2000;
				rotate_ref += 0;
			}
			if(RM_Q_Pressed){
				rot = true;

			}
					  if(RM_Z_Pressed){
							  rrot = true;
					  }
			if(RM_R_Pressed){
				rot = false;
							  rrot=false;
			}

		  //    calculate PID for rotation read data from gimbal
			pid_calc(&pid_rotation, motorYaw.angle, 7200);
			if(rot){
				rotation=-pid_rotation.pos_out;
			}
			else if(rrot){
							  rotation=pid_rotation.pos_out;
					  }
					  else{
							  rotation=0;
					  }
			rotate_ref += rotation;
		  //   The next few states are high-speed states
		  //   high-speed forward motion when pressing F
			if(RM_F_Pressed){
				forward_back_ref += 4000;
				left_right_ref += 0;
				rotate_ref += 0;
			}

		  // high-speed backward motion when pressing V
			if(RM_V_Pressed){
				forward_back_ref += -4000;
				left_right_ref += 0;
				rotate_ref += 0;
			}

		  // high-speed left motion when pressing C
			if(RM_C_Pressed){
				forward_back_ref += 0;
				left_right_ref += 4000;
				rotate_ref += 0;
			}

		  // high-speed right motion when pressing B
			if(RM_B_Pressed){
				forward_back_ref += 0;
				left_right_ref += -4000;
				rotate_ref += 0;
			}
}


/******************************************************************************
	Input: speed
	Output
	Description
		limit power on chassis
	Log
		2018/04/08 Ke Sun & Yaorui Hao
*******************************************************************************/
void power_limit_control(double* speed){

    /*ramp control method (current limiting) based on judgement system
    total_cur_limit needs to be defined according to the judgement system*/

    motor_measure_t motor_measurement;

    double total_cur = motor_measurement.torqueCurrent;

    if (total_cur > total_cur_limit)
    {
      speed[0] = speed[0] / total_cur * total_cur_limit;
      speed[1] = speed[1] / total_cur * total_cur_limit;
      speed[2] = speed[2] / total_cur * total_cur_limit;
      speed[3] = speed[3] / total_cur * total_cur_limit;
    }

}
