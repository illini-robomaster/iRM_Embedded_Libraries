/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

/**
 * @author  Nickel_Liang
 * @date    2018-04-12
 * @file    bsp_can.c
 * @brief   Board Support Package for CAN bus
 * @log     2018-04-13 Nickel_Liang
 */

#include "bsp_can.h"

uint8_t can1_rx_buffer[CAN1_DEVICE_NUM][CAN_DATA_SIZE];
uint8_t can2_rx_buffer[CAN2_DEVICE_NUM][CAN_DATA_SIZE];

void can1_init(void) {
    can_init(&CAN_BUS_1);
}

void can2_init(void) {
    can_init(&CAN_BUS_2);
}

void can1_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4) {
    can_transmit(&CAN_BUS_1, id, msg1, msg2, msg3, msg4);
}

void can2_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4) {
    can_transmit(&CAN_BUS_2, id, msg1, msg2, msg3, msg4);
}

uint8_t can1_read(uint16_t id, uint8_t buf[CAN_DATA_SIZE]) {
    uint8_t idx = id - CAN1_RX_ID_START;
    if (idx < 0 || idx >= CAN1_DEVICE_NUM) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Out of bound.");
        return 0;
    }
    /* Enter critical section here */
    /* @todo Critical Section not tested yet */
    /* @bug Actually not working. Try to use in a thread later */
    // taskENTER_CRITICAL();
    memcpy(buf, can1_rx_buffer[idx], CAN_DATA_SIZE);
    /* Exit critical section here */
    // taskEXIT_CRITICAL();
    return 1;
}

uint8_t can2_read(uint16_t id, uint8_t buf[CAN_DATA_SIZE]) {
    uint8_t idx = id - CAN2_RX_ID_START;
    if (idx < 0 || idx >= CAN2_DEVICE_NUM) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Out of bound.");
        return 0;
    }
    /* Enter critical section here */
    // taskENTER_CRITICAL();
    memcpy(buf, can2_rx_buffer[idx], CAN_DATA_SIZE);
    /* Exit critical section here */
    // taskEXIT_CRITICAL();
    return 1;
}

static void can_init(CAN_HandleTypeDef* hcan) {
    can_filter_config(hcan);   //Initialize filter 0

    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        bsp_error_handler(__FUNCTION__, __LINE__, "Cannot activate CAN notification");
    
    if (HAL_CAN_Start(hcan) != HAL_OK)
        bsp_error_handler(__FUNCTION__, __LINE__, "Cannot start CAN");
}

static void can_transmit(CAN_HandleTypeDef* hcan, uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4) {
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             data[8];
    uint32_t            pTxMailbox;

    tx_header.StdId = id;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = 0x08;
    data[0] = msg1 >> 8; 	//Higher 8 bits of ESC 1
    data[1] = msg1;		//Lower 8 bits of ESC 1
    data[2] = msg2 >> 8;
    data[3] = msg2;
    data[4] = msg3 >> 8;
    data[5] = msg3;
    data[6] = msg4 >> 8;
    data[7] = msg4;

    HAL_CAN_AddTxMessage(hcan, &tx_header, data, &pTxMailbox);
}

static void can_filter_config(CAN_HandleTypeDef* hcan) {
    CAN_FilterTypeDef       CAN_FilterConfigStructure;
    /* Configure Filter Property */
    CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
    CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
    CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterConfigStructure.FilterActivation = ENABLE;
    CAN_FilterConfigStructure.SlaveStartFilterBank = 14;  //CAN1 and CAN2 split all 28 filters
    /* Configure each CAN bus */
    if (hcan == &CAN_BUS_1)
        CAN_FilterConfigStructure.FilterBank = 0; //Master CAN1 get filter 0-13
    else if (hcan == &CAN_BUS_2)
        CAN_FilterConfigStructure.FilterBank = 14; //Slave CAN2 get filter 14-27

    if (HAL_CAN_ConfigFilter(hcan, &CAN_FilterConfigStructure) != HAL_OK)
        bsp_error_handler(__FUNCTION__, __LINE__, "CAN filter configuration failed.");
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    static CAN_RxHeaderTypeDef rx_header;
    /* get device id ahead of time */
    rx_header.StdId = (CAN_RI0R_STID & hcan->Instance->sFIFOMailBox[0].RIR) >> CAN_TI0R_STID_Pos;

    if (hcan == &CAN_BUS_1) {
        /* Enter critical section here */
        /* @todo Critical section not tested yet */
        UBaseType_t it_status = taskENTER_CRITICAL_FROM_ISR();
        uint8_t idx = rx_header.StdId - CAN1_RX_ID_START;
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, can1_rx_buffer[idx]);
        /* Exit critical section here */
        taskEXIT_CRITICAL_FROM_ISR(it_status);
    }
    else if (hcan == &CAN_BUS_2) {
        /* Enter critical section here */
        UBaseType_t it_status = taskENTER_CRITICAL_FROM_ISR();
        uint8_t idx = rx_header.StdId - CAN2_RX_ID_START;
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, can2_rx_buffer[idx]);
        /* Exit critical section here */
        taskEXIT_CRITICAL_FROM_ISR(it_status);
    }
}
