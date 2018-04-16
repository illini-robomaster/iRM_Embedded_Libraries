/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-12
 * @file    bsp_can.h
 * @brief   Board Support Package for CAN bus
 * @log     2018-04-13 nickelliang
 */

#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "can.h"
#include "bsp_error_handler.h"
#include <inttypes.h>
#include <string.h>

/**
 * @ingroup bsp
 * @defgroup bsp_can BSP CAN
 * @{
 */

#define CAN1_DEVICE_NUM     12
#define CAN2_DEVICE_NUM     12
#define CAN_DATA_SIZE       8
#define CAN1_RX_ID_START    0x201
#define CAN2_RX_ID_START    0x201

#define BUF_BUSY            1
#define BUF_OK              0

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
 * Interface for read CAN1 data
 *
 * @param  id         Node ID
 * @param  buf        Buffer want to copy to
 * @return            0 for failed
 *                    1 for success
 * @author Nickel_Liang
 * @date   2018-04-15
 */
uint8_t CAN1_read(uint16_t id, uint8_t buf[CAN_DATA_SIZE]);

/**
 * Interface for read CAN2 data
 *
 * @param  id         Node ID
 * @param  buf        Buffer want to copy to
 * @return            0 for failed
 *                    1 for success
 * @author Nickel_Liang
 * @date   2018-04-15
 */
uint8_t CAN2_read(uint16_t id, uint8_t buf[CAN_DATA_SIZE]);

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

/** @} */

#endif
