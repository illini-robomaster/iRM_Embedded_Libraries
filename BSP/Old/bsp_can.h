/******************************************************************************
	Description
		CAN Bus Utility
	Log
		11/23/17 Nickel Liang	First Draft
		12/03/17 Nickel Liang	Change pitch & yaw addr
		12/05/17 Nickel Liang	Add more comment
		12/06/17 Nickel Liang	Minor bug fix
		01/20/18 Nickel Liang	Modified CAN ID
*******************************************************************************/

#ifndef __BSP_CAN
#define __BSP_CAN

#include "mytype.h"

/* ID */
typedef enum {
	/* CAN1 TX ID */
	CAN_3508_1_4_TX = 0x200,	//3508 Motor 1-4 TX ID
	CAN_3508_5_8_TX = 0x1FF,	//3508 Motor 5-8 TX ID
	// CAN_6623_1_4_TX	= 0x1FF,	//6623 Motor 1-4 TX ID
	CAN_6623_1_4_TX = 0x2FF,	//6623 Motor 5-8 TX ID NOTE Should be 5_8
	CAN_6623_CLI_TX = 0x3F0,	//6623 Motor Calibration TX ID

	/* CAN1 RX ID */
	CAN_3508_1_RX 	= 0x201,	//Chassis Motor 1 RX ID
	CAN_3508_2_RX 	= 0x202,	//Chassis Motor 2 RX ID
	CAN_3508_3_RX 	= 0x203,	//Chassis Motor 3 RX ID
	CAN_3508_4_RX 	= 0x204,	//Chassis Motor 4 RX ID
	CAN_3508_5_RX 	= 0x205,	//Launcher Motor Left RX ID
	CAN_3508_6_RX 	= 0x206,	//Launcher Motor Right RX ID
	CAN_3508_7_RX 	= 0x207,	//Poke Motor RX ID
	// CAN_3508_8_RX 	= 0x208,	//

	// CAN_YAW_RX		= 0x205,	//6623-1, Yaw Motor RX ID
	// CAN_PITCH_RX		= 0x206,	//6623-2, Pitch Motor RX ID
	// CAN_ROLL_RX		= 0x207,	//6623-3
	// CAN_RESV_RX		= 0x208,	//6623-4
	CAN_YAW_RX		= 0x209,	//6623-5, NOTE Should be EX1_RX
	CAN_PITCH_RX	= 0x20A,	//6623-6, NOTE Should be EX2_RX
	// CAN_EX3_RX		= 0x20B,	//6623-7
	// CAN_EX4_RX		= 0x20C,	//6623-8

	/* CAN2 TX ID */
	/* CAN2 RX ID */
} RM_CAN_Message_ID;

/* Tx Message */
#define RM_CAN_6623_CLI_MSG	'c'	//6623 Motor Calibration Message

/* Rx Message
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
typedef struct {
	/* 3508 DATA[0]DATA[1], 6623 DATA[0]DATA[1] */
	uint16_t 	angle;				//Mechanical angle
	uint16_t 	lastAngle;	  		//Last mechanical angle
	uint16_t	offsetAngle; 		//Initial angle
	int32_t		totalAngle;			//Angle from initial position
	/* 3508 DATA[2]DATA[3] */
	int16_t 	speedRPM;			//Speed in rpm
	/* 3508 DATA[4]DATA[5], 6623 DATA[2]DATA[3] */
	int16_t		torqueCurrent;		//Actual torque current
	/* 3508 DATA[6] */
  	uint8_t  	temp;				//Temperature in Celcius
	/* 6623 DATA[4]DATA[5] */
	int16_t		givenCurrent;		//Given torque current
	/* General Counter */
	int32_t		roundCnt;			//Should be in range [-1,1]
	uint32_t	msgCnt;				//For initialization
} motor_measure_t;

/* Declaration */
extern motor_measure_t motorChassis[];
extern motor_measure_t motorYaw, motorPitch;
extern motor_measure_t motorLauncher[];
extern motor_measure_t motorPoke;

/* Global Functions */
void RM_CAN_SetMotor(CAN_HandleTypeDef* hcan, uint16_t id, int16_t spd1, int16_t spd2, int16_t spd3, int16_t spd4);
void RM_CAN_CalibrateGimbalMotor(CAN_HandleTypeDef* hcan);
void RM_CAN_InitCAN1(void);
void RM_CAN_InitCAN2(void);

/* Static Functions */
static void RM_CAN_FilterConfiguration(CAN_HandleTypeDef* hcan);
static void RM_CAN_GetChassisData(CAN_HandleTypeDef* hcan, motor_measure_t *ptr);
static void RM_CAN_GetGimbalData(CAN_HandleTypeDef* hcan, motor_measure_t *ptr);
static void RM_CAN_GetOffset(CAN_HandleTypeDef* hcan, motor_measure_t *ptr);

#endif
