/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-20
 * @file    test_bsp_imu.c
 * @brief   Test imu functionality
 * @log     2018-04-20 nickelliang
 */

#include "test_bsp_imu.h"

imu_t test_imu;

uint8_t test_bsp_imu(void) {
    for (size_t i = 0; i < IMU_TEST_COUNT; i++) {
        print_mpu_data(&test_imu);
    }
    return 1;
}
