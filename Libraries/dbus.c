/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-17
 * @file    dbus.c
 * @brief   DBUS utility
 * @log     2018-04-17 nickelliang
 */

#include "dbus.h"

uint8_t dbus_rx_buffer[DBUS_BUF_LEN];
dbus_t rc;

uint8_t dbus_init(void) {
    /* Initialize DBUS to IDLE interrupt */
    uart_port_init(&BSP_DBUS_PORT);
    /* Start DMA transfer with interrupt disabled */
    uint8_t ret = uart_rx_dma_without_it(&BSP_DBUS_PORT, dbus_rx_buffer, BSP_DBUS_MAX_LEN);
    /* Initialize DBUS struct */
    memset(dbus_get_struct(), 0, sizeof(dbus_t));
    return ret;
}

uint8_t dbus_data_process(uint8_t buff[DBUS_BUF_LEN], dbus_t* dbus) {
    /* Argument validity check */
    if (dbus == NULL) {
        bsp_error_handler(__FILE__, __LINE__, "Invalid DBUS struct.");
        return 0;
    }
    /* Read in rocker value */
    dbus->ch0 = (buff[0] | buff[1] << 8) & 0x07FF;
    dbus->ch1 = (buff[1] >> 3 | buff[2] << 5) & 0x07FF;
    dbus->ch2 = (buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF;
    dbus->ch3 = (buff[4] >> 1 | buff[5] << 7) & 0x07FF;
    /* Initialize rocker value to zero based */
    dbus->ch0 -= RC_ROCKER_MID;
    dbus->ch1 -= RC_ROCKER_MID;
    dbus->ch2 -= RC_ROCKER_MID;
    dbus->ch3 -= RC_ROCKER_MID;
    /* If rocker value blow up, probably something when wrong */
    if ((abs(dbus->ch0) > RC_ROCKER_MIN_MAX_DRIFT) || \
        (abs(dbus->ch1) > RC_ROCKER_MIN_MAX_DRIFT) || \
        (abs(dbus->ch2) > RC_ROCKER_MIN_MAX_DRIFT) || \
        (abs(dbus->ch3) > RC_ROCKER_MIN_MAX_DRIFT)) {
        memset(dbus, 0, sizeof(dbus_t));
        bsp_error_handler(__FILE__, __LINE__, "Rocker value blew up.");
        return 0;
    }
    /* Prevent rocker zero drift */
    if(abs(dbus->ch0) <= RC_ROCKER_ZERO_DRIFT)
        dbus->ch0 = 0;
    if(abs(dbus->ch1) <= RC_ROCKER_ZERO_DRIFT)
        dbus->ch1 = 0;
    if(abs(dbus->ch2) <= RC_ROCKER_ZERO_DRIFT)
        dbus->ch2 = 0;
    if(abs(dbus->ch3) <= RC_ROCKER_ZERO_DRIFT)
        dbus->ch3 = 0;
    /* Read in switch value */
    dbus->swl = ((buff[5] >> 4) & 0x000C) >> 2;
    dbus->swr = (buff[5] >> 4) & 0x0003;
    /* Read in mouse value */
    dbus->mouse.x = buff[6] | (buff[7] << 8);
    dbus->mouse.y = buff[8] | (buff[9] << 8);
    dbus->mouse.z = buff[10] | (buff[11] << 8);
    dbus->mouse.l = buff[12];
    dbus->mouse.r = buff[13];
    /* Check mouse data validity */
    if (abs(dbus->mouse.x) >= MOUSE_MAX) {
        bsp_error_handler(__FILE__, __LINE__, "Mouse X out of bound.");
        return 0;
    }
    if (abs(dbus->mouse.y) >= MOUSE_MAX) {
        bsp_error_handler(__FILE__, __LINE__, "Mouse Y out of bound.");
        return 0;
    }
    if (abs(dbus->mouse.z) >= MOUSE_MAX) {
        bsp_error_handler(__FILE__, __LINE__, "Mouse Z out of bound.");
        return 0;
    }
    /* Read in keyboard value */
    dbus->key.code = buff[14] | buff[15] << 8; // key borad code
    return 1;
}

inline dbus_t* dbus_get_struct(void) {
    return &rc;
}

/**
 * Callback function declared in bsp_uart. This is a weak function.
 *
 * @author Nickel_Liang
 * @date   2018-04-18
 */
void uart_dbus_callback(void) {
    /* Handle dbus data from DMA */
    /* Enter critical section here */
    /* @todo Critical Section not tested yet */
    UBaseType_t it_status = taskENTER_CRITICAL_FROM_ISR();
    if ((BSP_DBUS_MAX_LEN - dma_current_data_counter(BSP_DBUS_PORT.hdmarx->Instance)) == DBUS_BUF_LEN) {
        /* @todo Consider add signal handling here? */
        dbus_data_process(dbus_rx_buffer, dbus_get_struct());
        /* @todo Add offline detection for dbus */
    }
    /* Exit critical section here */
    taskEXIT_CRITICAL_FROM_ISR(it_status);
}
