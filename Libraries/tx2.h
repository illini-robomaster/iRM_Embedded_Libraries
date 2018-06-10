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
#define TX2_FIFO_SIZE   BSP_TX2_MAX_LEN
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
 * @param  target_struct  A valid tx2 structure
 * @param  process_struct A valid data process instance
 * @return                1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
uint8_t tx2_dispatcher(void* target_struct, data_process_t* process_struct);

/**
 * TX2 data packer. Used by data process lib.
 *
 * @param  target_struct  A valid tx2 structure
 * @param  process_struct A valid data process instance
 * @param  cmdid          Command ID of the msg
 * @return                1 for success, 0 for failed
 * @usage  tx2_packer(&tx2_info, tx2_process, CMD_AIM_REQUEST);
 * @author Nickel_Liang
 * @date   2018-05-26
 */
uint8_t tx2_packer(void *target_struct, data_process_t *process_struct, uint16_t cmdid);

/**
 * TX2 callback wrapper.
 *
 * @author Nickel_Liang
 * @date   2018-05-23
 */
void tx2_incomming(void);

/**
 * TX2 forward signal set wrapper.
 *
 * @author Nickel_Liang
 * @date   2018-05-26
 */
void tx2_outgoing(void);

#endif
