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
