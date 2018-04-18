/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-17
 * @file    test_bsp_can.h
 * @brief   Test print for can value
 * @log     2018-04-17 nickelliang
 */

#ifndef _TEST_BSP_CAN_H_
#define _TEST_BSP_CAN_H_

#include "bsp_can.h"
#include "bsp_print.h"

#define CAN_TEST_COUNT      100000
#define PRINT_CAN_1         1       // Set to 1 to print can 1 data
#define PRINT_CAN_2         0       // Set to 1 to print can 2 data

uint8_t test_bsp_can(void);

#endif
