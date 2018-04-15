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

#define CAN_CALLBACK_BUFFER_SIZE    9
#define CAN_CALLBACK_DATA_SIZE      8

/* RX buffer[8] = StdId, RX buffer[0:7] contains Data[0:7] */
extern uint8_t can1_rx_buffer[CAN_CALLBACK_BUFFER_SIZE];
extern uint8_t can2_rx_buffer[CAN_CALLBACK_BUFFER_SIZE];

/**
 * CAN1 init wrapper
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void CAN1_init(void);

/**
 * CAN2 init wrapper
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void CAN2_init(void);

/**
 * CAN1 transmit data
 *
 * @param  id         TX ID
 * @param  msg1       First message
 * @param  msg2       Second message
 * @param  msg3       Third message
 * @param  msg4       Fourth message
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void CAN1_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4);

/**
 * CAN2 transmit data
 *
 * @param  id         TX ID
 * @param  msg1       First message
 * @param  msg2       Second message
 * @param  msg3       Third message
 * @param  msg4       Fourth message
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void CAN2_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4);

/**
 * CAN initialization implementation
 *
 * @param  hcan       Which CAN to init
 * @author Nickel_Liang
 * @date   2018-04-14
 */
static void CAN_init(CAN_HandleTypeDef* hcan);

/**
 * CAN transmission implementation
 *
 * @param  hcan       Which CAN to init
 * @param  id         TX ID
 * @param  msg1       First message
 * @param  msg2       Second message
 * @param  msg3       Third message
 * @param  msg4       Fourth message
 * @author Nickel_Liang
 * @date   2018-04-14
 */
static void CAN_transmit(CAN_HandleTypeDef* hcan, uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4);

/**
 * Configure CAN filter to ACCEPT ALL incoming messages
 *
 * @param  hcan       Which CAN to configure
 * @author Nickel_Liang
 * @date   2018-04-14
 */
static void CAN_filter_config(CAN_HandleTypeDef* hcan);

#endif
