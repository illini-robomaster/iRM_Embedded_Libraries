/**
 * @author  Nickel_Liang
 * @date    2018-04-12
 * @file    bsp_can.c
 * @brief   Board Support Package for CAN bus
 * @log     2018-04-13 Nickel_Liang
 */

#include "bsp_can.h"

uint8_t can1_rx_buffer[CAN_CALLBACK_BUFFER_SIZE];
uint8_t can2_rx_buffer[CAN_CALLBACK_BUFFER_SIZE];

void CAN1_init(void) {
	CAN_init(&hcan1);
}

void CAN2_init(void) {
	CAN_init(&hcan2);
}

void CAN1_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4) {
	CAN_transmit(&hcan1, id, msg1, msg2, msg3, msg4);
}

void CAN2_transmit(uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4) {
	CAN_transmit(&hcan2, id, msg1, msg2, msg3, msg4);
}

static void CAN_init(CAN_HandleTypeDef* hcan) {
	CAN_filter_config(hcan);   //Initialize filter 0
	if (HAL_CAN_Receive_IT(hcan, CAN_FIFO0) != HAL_OK) {
		Error_Handler();
    }
}

static void CAN_transmit(CAN_HandleTypeDef* hcan, uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4) {
	hcan->pTxMsg->StdId = id;
	hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x08;
	hcan->pTxMsg->Data[0] = msg1 >> 8; 	//Higher 8 bits of ESC 1
	hcan->pTxMsg->Data[1] = msg1;		//Lower 8 bits of ESC 1
	hcan->pTxMsg->Data[2] = msg2 >> 8;
	hcan->pTxMsg->Data[3] = msg2;
	hcan->pTxMsg->Data[4] = msg3 >> 8;
	hcan->pTxMsg->Data[5] = msg3;
	hcan->pTxMsg->Data[6] = msg4 >> 8;
	hcan->pTxMsg->Data[7] = msg4;
	HAL_CAN_Transmit(hcan, 1000);
}

static void CAN_filter_config(CAN_HandleTypeDef* hcan) {
	CAN_FilterConfTypeDef	CAN_FilterConfigStructure;

	static CanTxMsgTypeDef	Tx1Message;	//Allocate memory for data storage
	static CanRxMsgTypeDef 	Rx1Message;
	static CanTxMsgTypeDef	Tx2Message;
	static CanRxMsgTypeDef 	Rx2Message;

	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterActivation = ENABLE;
	CAN_FilterConfigStructure.BankNumber = 14;	//CAN1 and CAN2 split all 28 filters

	if (hcan == &hcan1) {
		CAN_FilterConfigStructure.FilterNumber = 0; //Master CAN1 get filter 0-13
		hcan->pTxMsg = &Tx1Message;
		hcan->pRxMsg = &Rx1Message;
	} else if (hcan == &hcan2) {
		CAN_FilterConfigStructure.FilterNumber = 14; //Slave CAN2 get filter 14-27
		hcan->pTxMsg = &Tx2Message;
		hcan->pRxMsg = &Rx2Message;
	}

	if (HAL_CAN_ConfigFilter(hcan, &CAN_FilterConfigStructure) != HAL_OK) {
		Error_Handler();
    }
}

/**
 * CAN bus callback function, no need to define in h file. This will overwrite
 *
 * @param  hcan       [description]
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan) {
	if (hcan == &hcan1) {
		memcpy(can1_rx_buffer, hcan->pRxMsg->Data, CAN_CALLBACK_DATA_SIZE);
		can1_rx_buffer[CAN_CALLBACK_BUFFER_SIZE - 1] = hcan->pRxMsg->StdId;
	}
	else if (hcan == &hcan2) {
		memcpy(can2_rx_buffer, hcan->pRxMsg->Data, CAN_CALLBACK_DATA_SIZE);
		can2_rx_buffer[CAN_CALLBACK_BUFFER_SIZE - 1] = hcan->pRxMsg->StdId;
	}
	// Reset CAN receive interrupt to prevent bug
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);
	__HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);
}
