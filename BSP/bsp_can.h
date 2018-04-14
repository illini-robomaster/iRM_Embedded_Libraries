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
#include "main.h"
#include "can.h"
#include <inttypes.h>

void CAN1_init(void);

void CAN2_init(void);

void CAN1_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4);

void CAN2_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4);

static void CAN_init(CAN_HandleTypeDef* hcan);

static void CAN_transmit(CAN_HandleTypeDef* hcan, uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4);

static void CAN_filter_config(CAN_HandleTypeDef* hcan);

static void RM_CAN_GetChassisData(CAN_HandleTypeDef* hcan, motor_measure_t *ptr);

static void RM_CAN_GetGimbalData(CAN_HandleTypeDef* hcan, motor_measure_t *ptr);
static void RM_CAN_GetOffset(CAN_HandleTypeDef* hcan, motor_measure_t *ptr);

#endif
