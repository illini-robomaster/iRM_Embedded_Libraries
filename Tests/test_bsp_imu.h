/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-20
 * @file    test_bsp_imu.h
 * @brief   Test imu functionality
 * @log     2018-04-20 nickelliang
 */

#ifndef _TEST_BSP_IMU_H_
#define _TEST_BSP_IMU_H_

#include "bsp_imu.h"
#include "bsp_print.h"
#include "imu_onboard.h"

#define IMU_TEST_COUNT     100000

uint8_t test_bsp_imu(void);

#endif
