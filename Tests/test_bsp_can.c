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
 * @date    2018-04-17
 * @file    test_bsp_can.c
 * @brief   Test print for can value
 * @log     2018-04-17 nickelliang
 */

#include "test_bsp_can.h"

extern uint8_t can1_rx_buffer[CAN1_DEVICE_NUM][CAN_DATA_SIZE];
extern uint8_t can2_rx_buffer[CAN2_DEVICE_NUM][CAN_DATA_SIZE];

uint8_t test_bsp_can(void) {
    uint8_t ret = 1;
    size_t i;

    for (i = 0; i < CAN_TEST_COUNT; i++) {
        size_t device;
        if (PRINT_CAN_1) {
            print("===CAN1===");
            for (device = 0; device < CAN1_DEVICE_NUM; device++) {
                print("Node %d: %08x\r\n", device, can1_rx_buffer[device]);
            }
        }
        print("==========");
        if (PRINT_CAN_2) {
            print("===CAN2===");
            for (device = 0; device < CAN2_DEVICE_NUM; device++) {
                print("Node %d: %08x\r\n", device, can2_rx_buffer[device]);
            }
            print("==========");
        }
    }

    return ret;
}
