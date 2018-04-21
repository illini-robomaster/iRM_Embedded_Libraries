/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-20
 * @file    bsp_imu.h
 * @brief   Board support package for IMU
 * @log     2018-04-20 nickelliang
 */

#ifndef _BSP_IMU_H
#define _BSP_IMU_H

#include "bsp_mpu6500_reg.h"
#include "bsp_ist8310_reg.h"
#include "bsp_error_handler.h"
#include "bsp_print.h"
#include "spi.h"
#include "main.h"

/* Constants and Common Functions */
#define RAD_2_DEG 57.29578
#define DEG_2_RAD 0.017453
#define MY_ABS(x) ((x>0) ? (x) : (-x))

#define ONBOARD_IMU_SPI     hspi5
#define ONBOARD_IMU_TIMEOUT 55
#define ONBOARD_IMU_BUFFER  14
#define ONBOARD_IMU_WRITE   0x7f
#define ONBOARD_IMU_READ    0x80
#define ONBOARD_ACCE_FACTOR 4096.0f // Check datasheet, 8g = 4096
#define ONBOARD_TEMP_ROOM   21      // Roomtemp
#define ONBOARD_TEMP_OFFSET 0
#define ONBOARD_TEMP_FACTOR 333.87f // Datasheet p12
#define ONBOARD_GYRO_FACTOR 16.384f // Check datasheet, 2000dps = 16.384

typedef struct {
    struct {
        float x;
        float y;
        float z;
    } __packed acce;
    // In celcius
    float temp;
    // In degree
    struct {
        float x;
        float y;
        float z;
    } __packed gyro;
    // Raw value of magnetomitor
    struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
    } __packed mag;
} __packed imu_t;

/**
 * Initialize onboard imu
 *
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
uint8_t onboard_imu_init(void);

/**
 * Get accelerometer and gyroscope data
 *
 * @param  imu        A valid imu object
 * @author Nickel_Liang
 * @date   2018-04-21
 */
void mpu6500_get_data(imu_t* imu);

/**
 * Get magnetomitor data
 *
 * @param  imu        A valid imu object
 * @author Nickel_Liang
 * @date   2018-04-21
 */
void ist8310_get_data(imu_t* imu);

/**
 * Print accelerometer, gyroscope, temp data
 *
 * @param  imu        A valid imu object
 * @author Nickel_Liang
 * @date   2018-04-21
 */
void print_mpu_data(imu_t* imu);

/**
 * Initialize MPU6500
 *
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t mpu6500_init(void);

/**
 * Write MPU6500 register
 *
 * @param  reg        Register to write
 * @param  data       Data to write
 * @return            1
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t mpu6500_write_reg(uint8_t const reg, uint8_t const data);

/**
 * Read MPU6500 register
 *
 * @param  reg        Register to write
 * @return            Data received
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t mpu6500_read_reg(uint8_t const reg);

/**
 * Read a sequence of MPU6500 registers
 *
 * @param  reg_addr   Starting address of registers to read
 * @param  data       Rx buffer
 * @param  len        Length of data
 * @return            1
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t mpu6500_read_regs(uint8_t const reg_addr, uint8_t* data, uint8_t len);

/**
 * Initialize IST8310
 *
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t ist8310_init(void);

/**
 * Read IST8310 register
 *
 * @param  addr       Register address to read
 * @return            Register data
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t ist8310_read_reg(uint8_t addr);

/**
 * Write IST8310 register
 *
 * @param  addr       Register address to write
 * @param  data       Register data
 * @return            1
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static uint8_t ist8310_write_reg(uint8_t addr, uint8_t data);

/**
 * Configure IST8310 read throught MPU6500 mode
 *
 * @param  device_address Address of IST8310
 * @param  reg_base_addr  Base address of IST8310
 * @param  data_num       Number of data for each auto read
 * @author Nickel_Liang
 * @date   2018-04-21
 */
static void mpu_read_ist_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);

#endif
