/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-12
 * @file    bsp_can.h
 * @brief   Board Support Package for CAN bus
 * @log     2018-04-13 nickelliang
 */

#ifndef _BSP_CAN_
#define _BSP_CAN_

#include "stm32f4xx_hal.h"
#include "rm_config.h"

#define MOTOR_6623_CLI_MSG            'c' //6623 Motor Calibration Message

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
