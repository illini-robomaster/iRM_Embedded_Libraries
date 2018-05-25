/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    tx2.h
 * @brief   TX2 transmit / receive utility
 * @log     2018-04-18 nickelliang
 */

#ifndef _TX2_H_
#define _TX2_H_

#include "bsp_error_handler.h"
#include "bsp_uart.h"
#include "bsp_config.h"
#include "data_process.h"

#define TX2_SOF         0xA0
#define TX2_PORT        BSP_TX2_PORT
#define TX2_FIFO_SIZE   (BSP_TX2_MAX_LEN / 2)
#define TX2_BUFF_SIZE   BSP_TX2_MAX_LEN

typedef enum {
    CMD_GIMBAL_CONTROL  = 0x00A1,
    CMD_AIM_REQUEST     = 0x0012,
} tx2_cmdid_t;

/* ===== CMD_GIMBAL_CONTROL 0x00A1 ===== */
typedef struct {
    uint16_t    pitch_ref;
    uint16_t    yaw_ref;
} __packed gimbal_control_t;

/* ===== CMD_AIM_REQUEST 0x0012 ===== */
typedef struct {
    uint8_t     aim_mode;
} __packed aim_request_t;

typedef enum {
    RUNE        = 0,
    AUTOAIM     = 1,
} aim_mode_t;

/* ============================== */

typedef struct {
    gimbal_control_t    gimbal_control;     // 0x00A1
    aim_request_t       aim_request;        // 0x0012
} tx2_t;

/**
 * Initialize tx2 dma
 *
 * @param  source   A valid data process instance
 * @return 1 for success, 0 for error
 * @author Nickel_Liang
 * @date   2018-04-19
 */
uint8_t tx2_init(data_process_t* source);

/**
 * TX2 data dispatcher. Used by data process lib.
 *
 * @param  referee    A valid tx2 structure
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
uint8_t tx2_dispatcher(void* target_struct, data_process_t* process_struct);

/**
 * TX2 callback wrapper.
 *
 * @author Nickel_Liang
 * @date   2018-05-23
 */
__weak void tx2_callback(void);

#endif
