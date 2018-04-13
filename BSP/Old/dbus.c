/******************************************************************************
	Description
		DBUS Utility, Connect to USART1 PB7 on RM Standard Board
    Log
		12/02/17 Nickel Liang	Copy code from instruction manual
		12/05/17 Nickel Liang	Change in Callback function
        12/06/17 Nickel Liang   Move Callback function to bsp_uart.c/h
                                DBUS with IT is working
        12/07/17 Nickel Liang   Add Debug Utility
        12/08/17 Nickel Liang   Fix serial error bug
        12/19/17 Nickel Liang   Add Init Check
        12/20/17 Nickel Liang   Printf Function updated
*******************************************************************************/

#include "dbus.h"

dbus_t remoteData;
uint8_t dbusRxData[DBUS_RAW_LEN];

/******************************************************************************
	Input
	Output
	Description
		Dbus initialization
	Log
        12/06/17 Nickel Liang   First Draft. Interrupt service is proved
                                to be working.
        12/19/17 Nickel Liang   Add Init Check
*******************************************************************************/
void RM_DBUS_Init(void) {
    /*******************************************************
    NOTE This part is proved to be functional.
    *******************************************************/
    if (HAL_UART_Receive_IT(&huart1, dbusRxData, DBUS_RAW_LEN) != HAL_OK) {
        Error_Handler();
    }
    RM_DBUS_InitCheck();
    /******************************************************/

    // if (HAL_UART_Receive_DMA(&huart1, uart1_rx_buff, 18) != HAL_OK) {
    // 	Error_Handler();
    // }
    // RM_PRINTF("receive dma\r\n");
}

/******************************************************************************
	Input
		data stream
	Output
	Description
		Decode and read controller sent data
	Log
		12/02/17 Nickel Liang	Copy code from instruction manual
		12/05/17 Nickel Liang	Add comments
        12/06/17 Nickel Liang   Minor improvements
        12/06/17 Nickel Liang   Bug Fix
    NOTE
        S1 Channel is not working properly
        Mouse X & Y are not 16 bits but 14 bits
    TODO
        Add prevention measure. Sometimes the datashift might be problematic.
*******************************************************************************/
void RM_DBUS_DataProcess(uint8_t *rxData) {
    if(rxData != NULL) {
        remoteData.rc.ch0       = ((uint16_t)rxData[0] | ((uint16_t)rxData[1] << 8)) & 0x07FF;
        remoteData.rc.ch1       = (((uint16_t)rxData[1] >> 3) | ((uint16_t)rxData[2] << 5)) & 0x07FF;
        remoteData.rc.ch2       = (((uint16_t)rxData[2] >> 6) | ((uint16_t)rxData[3] << 2) | ((uint16_t)rxData[4] << 10)) & 0x07FF;
        remoteData.rc.ch3       = (((uint16_t)rxData[4] >> 1) | ((uint16_t)rxData[5] << 7)) & 0x07FF;
        remoteData.rc.s1        = ((rxData[5] >> 4) & 0x000C) >> 2;
        remoteData.rc.s2        = ((rxData[5] >> 4) & 0x0003);

        remoteData.mouse.x      = (((uint16_t)rxData[6]) | ((uint16_t)rxData[7] << 7));
        if ((rxData[7] & 0x70) > 0) remoteData.mouse.x |= 0xC000;   // Noticed Client only sent 15 bits
        remoteData.mouse.y      = (((uint16_t)rxData[8]) | ((uint16_t)rxData[9] << 7));
        if ((rxData[9] & 0x70) > 0) remoteData.mouse.y |= 0xC000;   // Don't use left shift 1 bit
        remoteData.mouse.z      = (((uint16_t)rxData[10]) | ((uint16_t)rxData[11] << 7));
        if ((rxData[11] & 0x70) > 0) remoteData.mouse.z |= 0xC000;  // it will cause larger negative number

        remoteData.mouse.pressL = rxData[12];
        remoteData.mouse.pressR = rxData[13];

        remoteData.key.raw      = ((uint16_t)rxData[14]) | ((uint16_t)rxData[15] << 8);
        remoteData.key.w        = remoteData.key.raw & DBUS_W;
        remoteData.key.s        = remoteData.key.raw & DBUS_S;
        remoteData.key.a        = remoteData.key.raw & DBUS_A;
        remoteData.key.d        = remoteData.key.raw & DBUS_D;
        remoteData.key.shift    = remoteData.key.raw & DBUS_SHIFT;
        remoteData.key.ctrl     = remoteData.key.raw & DBUS_CTRL;
        remoteData.key.q        = remoteData.key.raw & DBUS_Q;
        remoteData.key.e        = remoteData.key.raw & DBUS_E;
        remoteData.key.r        = remoteData.key.raw & DBUS_R;
        remoteData.key.f        = remoteData.key.raw & DBUS_F;
        remoteData.key.g        = remoteData.key.raw & DBUS_G;
        remoteData.key.z        = remoteData.key.raw & DBUS_Z;
        remoteData.key.x        = remoteData.key.raw & DBUS_X;
        remoteData.key.c        = remoteData.key.raw & DBUS_C;
        remoteData.key.v        = remoteData.key.raw & DBUS_V;
        remoteData.key.b        = remoteData.key.raw & DBUS_B;
    }
}

/******************************************************************************
	Input
	Output
	Description
		Print dbus received raw data
	Log
        12/07/17 Nickel Liang   First Draft.
*******************************************************************************/
void RM_DEBUG_DBUS_PrintRaw(void) {
    RM_PRINTF("dbusRxData %02x%02x %02x%02x ", dbusRxData[0],  dbusRxData[1],  dbusRxData[2],  dbusRxData[3]);
    RM_PRINTF("%02x%02x %02x%02x ", dbusRxData[4],  dbusRxData[5],  dbusRxData[6],  dbusRxData[7]);
    RM_PRINTF("%02x%02x %02x%02x ", dbusRxData[8],  dbusRxData[9],  dbusRxData[10],  dbusRxData[11]);
    RM_PRINTF("%02x%02x %02x%02x ", dbusRxData[12],  dbusRxData[13],  dbusRxData[14],  dbusRxData[15]);
    RM_PRINTF("%02x%02x\r\n", dbusRxData[16],  dbusRxData[17]);
}

/******************************************************************************
	Input
	Output
	Description
		Print decoded dbus data
	Log
        12/07/17 Nickel Liang   First Draft.
*******************************************************************************/
void RM_DEBUG_DBUS_PrintValue(void) {
    RM_PRINTF("CH0 %u CH1 %u CH2 %u CH3 %u | ", remoteData.rc.ch0, remoteData.rc.ch1, remoteData.rc.ch2, remoteData.rc.ch3);
    RM_PRINTF(" S1 %u S2 %u |", remoteData.rc.s1, remoteData.rc.s2);
    RM_PRINTF(" Mouse X %d Mouse Y %d |", remoteData.mouse.x, remoteData.mouse.y);
    RM_PRINTF(" Click L %u Click R %u |", remoteData.mouse.pressL, remoteData.mouse.pressR);
    RM_PRINTF("KEY %04x\r\n", remoteData.key.raw);
}

/******************************************************************************
	Input
	Output
	Description
		Print decoded keyboard & mouse action
	Log
        12/07/17 Nickel Liang   First Draft.
*******************************************************************************/
void RM_DEBUG_DBUS_PrintAction(void) {
    if (RM_M_Action) {  // If mouse moved
        if (RM_Move_Left)       {RM_PRINTF("< ");}   else {RM_PRINTF("x ");}
        if (RM_Move_Up)         {RM_PRINTF("^ ");}   else {RM_PRINTF("x ");}
        if (RM_Move_Right)      {RM_PRINTF("> ");}   else {RM_PRINTF("x ");}
        if (RM_Move_Down)       {RM_PRINTF("v | ");} else {RM_PRINTF("x | ");}
        if (RM_Left_Clicked)    {RM_PRINTF("L ");}   else {RM_PRINTF("x ");}
        if (RM_Right_Clicked)   {RM_PRINTF("R ");}   else {RM_PRINTF("x ");}
    }
    if (RM_M_Action && RM_K_Action) {
        RM_PRINTF("| ");
    }
    if (RM_K_Action) {  // If key pressed
        if (RM_SHIFT_Pressed)   {RM_PRINTF("SHIFT ");}
        if (RM_CTRL_Pressed)    {RM_PRINTF("CTRL ");}
        if (RM_W_Pressed)       {RM_PRINTF("W ");}
        if (RM_S_Pressed)       {RM_PRINTF("S ");}
        if (RM_A_Pressed)       {RM_PRINTF("A ");}
        if (RM_D_Pressed)       {RM_PRINTF("D ");}
        if (RM_Q_Pressed)       {RM_PRINTF("Q ");}
        if (RM_E_Pressed)       {RM_PRINTF("E ");}
        if (RM_R_Pressed)       {RM_PRINTF("R ");}
        if (RM_F_Pressed)       {RM_PRINTF("F ");}
        if (RM_G_Pressed)       {RM_PRINTF("G ");}
        if (RM_Z_Pressed)       {RM_PRINTF("Z ");}
        if (RM_X_Pressed)       {RM_PRINTF("X ");}
        if (RM_C_Pressed)       {RM_PRINTF("C ");}
        if (RM_V_Pressed)       {RM_PRINTF("V ");}
        if (RM_B_Pressed)       {RM_PRINTF("B ");}
    }
    if (RM_M_Action || RM_K_Action) {   // If mouse or keyboard triggered
        RM_PRINTF("\r\n");
    }
}

/******************************************************************************
	Input
	Output
	Description
		Check if we got correct data from uart1
        This function shoule be more considerate. E.g. add a retry counter etc.
	Log
        12/08/17 Nickel Liang   First Draft.
        12/19/17 Nickel Liang   Remove msgCnt
*******************************************************************************/
void RM_DBUS_InitCheck(void) {
    if (remoteData.mouse.x != 0 || remoteData.mouse.y != 0 || remoteData.key.raw != 0) { // Check data receive validity
        HAL_Delay(100);
        // TODO add error handler
        RM_LED_RedOn();
        RM_LED_GreenOff();
        while (1) {
            RM_BUZZER_SingTone(Si7M, 1);
            HAL_Delay(10);
            RM_BUZZER_SingTone(Silent, 1);
            HAL_Delay(1000);
        }
    }
}
