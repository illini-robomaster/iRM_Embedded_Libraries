/******************************************************************************
	Description
		UART BSP
    Log
		12/06/17 Nickel Liang	First Draft
*******************************************************************************/

#include "bsp_uart.h"

/******************************************************************************
	Input
		UART_HandleTypeDef *huart
	Output
	Description
		CALLBACK FUNCTION. DO NOT DECLARE.
        Full length callback function of uart
	Log
		12/02/17 Nickel Liang	Copy code from instruction manual
		12/05/17 Nickel Liang	Check if data stream come from huart
        12/06/17 Nickel Liang   DBUS Interrupt Supported
                                Moved from dbus.c to here
        03/05/18 Alvin Sun      Enable uart3 for protocal communication
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    RM_PRINTF("CALL_BACK\t");
    /*******************************************************
    NOTE This part is proved to be functional.
    *******************************************************/
    if (huart == &huart1) {
        RM_DBUS_DataProcess(dbusRxData);
        if (HAL_UART_Receive_IT(&huart1, dbusRxData, DBUS_RAW_LEN) != HAL_OK) {
            Error_Handler();
        }
        __HAL_UART_CLEAR_PEFLAG(&huart1);
    }

    /******************************************************/

    if (huart == &huart3){
        if(CheckProStatus() == STATUS_DATA_INVALID)
            if (protoDataProc() == REQUEST_FAIL)
                Error_Handler();

        if (HAL_UART_Receive_IT(&huart3, protoReceiveData, PROTO_BUFF_LENGTH)!=HAL_OK)
            Error_Handler();

        __HAL_UART_CLEAR_PEFLAG(&huart3);
    }

    //__HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);
    // int i = 0;
    /*HAL_UART_Receive_DMA(&huart1, (uint8_t *)&rxString, 18);
    printf("%u\r\n", rxBuffer);*/
    // printf("Here in the interrupt\r\n");
    /*if (huart->Instance == USART1) {
        printf("Interrupt from USART1\r\n");
        RemoteDataProcess((uint8_t *)&rxBuffer);
    }
    rxString[rxindex] = rxBuffer; // Add that character to the string
    rxindex++;
    if (rxindex > MAXSTRING) {// User typing too much, we can't have commands that big

      rxindex = 0;
      for (int i = 0; i < MAXSTRING; i++) rxString[i] = 0; // Clear the string buffer
      printf("\r\nKonsole> ");
    }*/
    // if(huart == &huart1)
    // {
    // __HAL_UART_CLEAR_PEFLAG(&huart1);
    // HAL_UART_Receive_DMA(&huart1, uart1_rx_buff, 18);
    // }
}
