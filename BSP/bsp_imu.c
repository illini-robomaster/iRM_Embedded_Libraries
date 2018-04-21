/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-20
 * @file    bsp_imu.c
 * @brief   Board support package for IMU
 * @log     2018-04-20 nickelliang
 */

#include "bsp_imu.h"

#define ONBOARD_NSS_LOW     HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)
#define ONBOARD_NSS_HIGH    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)
#define RESET_IST           HAL_GPIO_WritePin(GPIOE, IST_RST_Pin, GPIO_PIN_SET)

static uint8_t mpu_tx, mpu_rx;

uint8_t onboard_imu_init(void) {
    if (mpu6500_init() == 0) {
        bsp_error_handler(__FUNCTION__, __LINE__, "MPU6500 init failed.");
        return 0;
    }
    if (ist8310_init() == 0) {
        bsp_error_handler(__FUNCTION__, __LINE__, "IST8310 init failed.");
        return 0;
    }
    return 1;
}

void mpu6500_get_data(imu_t* imu) {
    if (imu == NULL) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Invalid imu object.");
        return;
    }
    uint8_t mpu_rx_buff[ONBOARD_IMU_BUFFER];
    /* Must read 14 regs all together */
    mpu6500_read_regs(MPU6500_ACCEL_XOUT_H, mpu_rx_buff, ONBOARD_IMU_BUFFER);

    int16_t imu_acce_x_raw = (int16_t)(mpu_rx_buff[0] << 8 | mpu_rx_buff[1]);
    int16_t imu_acce_y_raw = (int16_t)(mpu_rx_buff[2] << 8 | mpu_rx_buff[3]);
    int16_t imu_acce_z_raw = (int16_t)(mpu_rx_buff[4] << 8 | mpu_rx_buff[5]);
    imu->acce.x = (float)(imu_acce_x_raw / ONBOARD_ACCE_FACTOR);
    imu->acce.y = (float)(imu_acce_y_raw / ONBOARD_ACCE_FACTOR);
    imu->acce.z = (float)(imu_acce_z_raw / ONBOARD_ACCE_FACTOR);

    int16_t imu_temp_raw   = (int16_t)(mpu_rx_buff[6] << 8 | mpu_rx_buff[7]);
    imu->temp   = (float)(((imu_temp_raw - ONBOARD_TEMP_OFFSET) / ONBOARD_TEMP_FACTOR) + ONBOARD_TEMP_ROOM);  // Eq in register map p33

    int16_t imu_gyro_x_raw = (int16_t)(mpu_rx_buff[8] << 8 | mpu_rx_buff[9]);
    int16_t imu_gyro_y_raw = (int16_t)(mpu_rx_buff[10] << 8 | mpu_rx_buff[11]);
    int16_t imu_gyro_z_raw = (int16_t)(mpu_rx_buff[12] << 8 | mpu_rx_buff[13]);
    imu->gyro.x = (float)(imu_gyro_x_raw / ONBOARD_GYRO_FACTOR);
    imu->gyro.y = (float)(imu_gyro_y_raw / ONBOARD_GYRO_FACTOR);
    imu->gyro.z = (float)(imu_gyro_z_raw / ONBOARD_GYRO_FACTOR);
}

void ist8310_get_data(imu_t* imu) {
    if (imu == NULL) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Invalid imu object.");
        return;
    }
    uint8_t ist_buff[6];
    mpu6500_read_regs(MPU6500_EXT_SENS_DATA_00, ist_buff, 6);
    imu->mag.x = (int16_t)(ist_buff[1]<<8 | ist_buff[0]);
    imu->mag.y = (int16_t)(ist_buff[3]<<8 | ist_buff[2]);
    imu->mag.z = (int16_t)(ist_buff[5]<<8 | ist_buff[4]);
}

void print_mpu_data(imu_t* imu) {
    if (imu == NULL) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Invalid imu object.");
        return;
    }
    mpu6500_get_data(imu);
    print("[DECODED MPU] ");
    print("Acce X %f \tY %f \t", imu->acce.x, imu->acce.y);
    print("Z %f \t| ", imu->acce.z);
    print("Gyro X %f \tY %f \t", imu->gyro.x, imu->gyro.y);
    print("Z %f \t| ", imu->gyro.z);
    print("Temp %f\r\n", imu->temp);
}

/* Static function for MPU6500 */
static uint8_t mpu6500_init(void) {
    // Reset the internal registers
    mpu6500_write_reg(MPU6500_PWR_MGMT_1, 0x80);
    HAL_Delay(100);
    // Reset gyro/accel/temp digital signal path
    mpu6500_write_reg(MPU6500_SIGNAL_PATH_RESET, 0x07);
    HAL_Delay(100);
    if (MPU6500_ID != mpu6500_read_reg(MPU6500_WHO_AM_I)) {
        bsp_error_handler(__FUNCTION__, __LINE__, "MPU6500 ID does not match.");
        return 0;
    }
    uint8_t MPU6500_Init_Data[7][2] = {
        {MPU6500_PWR_MGMT_1,     0x03},      // Clock Source - Gyro-Z
        {MPU6500_PWR_MGMT_2,     0x00},      // Enable Acc & Gyro
        {MPU6500_CONFIG,         0x04},      // gyro bandwidth 184Hz 01
        {MPU6500_GYRO_CONFIG,    0x18},      // 0x00 = 250dps / factor 131, 0x08 = 500dps / 65.5, 0x10 = 1000dps / 32.8, 0x18 = 2000dps / 16.4
        {MPU6500_ACCEL_CONFIG,   0x10},      // 0x00 = 2g / 16384, 0x08 = 4g / 8192, 0x10 = 8g / 4096, 0x18 = 16g / 2048
        {MPU6500_ACCEL_CONFIG_2, 0x04},      // acc bandwidth 20Hz
        {MPU6500_USER_CTRL,      0x20},      // Enable the I2C Master I/F module, pins ES_DA and ES_SCL are isolated from pins SDA/SDI and SCL/SCLK.
    };
    uint8_t index = 0;
    for(index = 0; index < 7; index++) {
        mpu6500_write_reg(MPU6500_Init_Data[index][0], MPU6500_Init_Data[index][1]);
        HAL_Delay(1);
    }
    return 1;
}

static uint8_t mpu6500_write_reg(uint8_t const reg, uint8_t const data) {
    ONBOARD_NSS_LOW;
    mpu_tx = reg & 0x7f;
    HAL_SPI_TransmitReceive(&ONBOARD_IMU_SPI, &mpu_tx, &mpu_rx, 1, ONBOARD_IMU_TIMEOUT);
    mpu_tx = data;
    HAL_SPI_TransmitReceive(&ONBOARD_IMU_SPI, &mpu_tx, &mpu_rx, 1, ONBOARD_IMU_TIMEOUT);
    ONBOARD_NSS_HIGH;
    return 1;
}

static uint8_t mpu6500_read_reg(uint8_t const reg) {
    ONBOARD_NSS_LOW;
    mpu_tx = reg | 0x80;
    HAL_SPI_TransmitReceive(&ONBOARD_IMU_SPI, &mpu_tx, &mpu_rx, 1, ONBOARD_IMU_TIMEOUT);
    HAL_SPI_TransmitReceive(&ONBOARD_IMU_SPI, &mpu_tx, &mpu_rx, 1, ONBOARD_IMU_TIMEOUT);
    ONBOARD_NSS_HIGH;
    return mpu_rx;
}

static uint8_t mpu6500_read_regs(uint8_t const reg_addr, uint8_t* data, uint8_t len) {
    uint8_t mpu_tx_buff[ONBOARD_IMU_BUFFER] = {0xFF};
    ONBOARD_NSS_LOW;
    mpu_tx = reg_addr | 0x80;
    mpu_tx_buff[0] = mpu_tx;
    HAL_SPI_TransmitReceive(&ONBOARD_IMU_SPI, &mpu_tx, &mpu_rx, 1, ONBOARD_IMU_TIMEOUT);
    HAL_SPI_TransmitReceive(&ONBOARD_IMU_SPI, mpu_tx_buff, data, len, ONBOARD_IMU_TIMEOUT);
    ONBOARD_NSS_HIGH;
    return 1;
}

/* Static function for IST8310 */
static uint8_t ist8310_init(void) {
    // Enable the I2C Master I/F module, Reset I2C Slave module
    mpu6500_write_reg(MPU6500_USER_CTRL, 0x30);
    HAL_Delay(10);
    // I2C master clock 400kHz
    mpu6500_write_reg(MPU6500_I2C_MST_CTRL, 0x0D);
    HAL_Delay(10);
    // Turn on slave 1 for ist write and slave 4 for ist read
    mpu6500_write_reg(MPU6500_I2C_SLV1_ADDR, IST8310_ADDRESS); // Write from slave 1
    HAL_Delay(10);
    mpu6500_write_reg(MPU6500_I2C_SLV4_ADDR, 0x80 | IST8310_ADDRESS); // Read from slave 4
    HAL_Delay(10);
    // reset ist8310
    ist8310_write_reg(IST8310_R_CONFB, 0x01);
    HAL_Delay(10);
    // Check IST id
    if(IST8310_DEVICE_ID_A != ist8310_read_reg(IST8310_WHO_AM_I)) {
        bsp_error_handler(__FUNCTION__, __LINE__, "IST8310 ID does not match.");
        return 0;
    }
    // Reset ist8310
    ist8310_write_reg(IST8310_R_CONFB, 0x01);
    HAL_Delay(10);
    // Config as ready mode to access reg
    ist8310_write_reg(IST8310_R_CONFA, 0x00);
    if(ist8310_read_reg(IST8310_R_CONFA) != 0x00) {
        bsp_error_handler(__FUNCTION__, __LINE__, "IST ready mode failed.");
        return 0;
    }
    HAL_Delay(10);
    // Normal state, no int
    ist8310_write_reg(IST8310_R_CONFB, 0x00);
    if(ist8310_read_reg(IST8310_R_CONFB) != 0x00) {
        bsp_error_handler(__FUNCTION__, __LINE__, "IST normal state init failed.");
        return 0;
    }
    HAL_Delay(10);
    // Config low noise mode, x,y,z axis 16 time 1 avg,
    ist8310_write_reg(IST8310_AVGCNTL, 0x24); // 100100
    if(ist8310_read_reg(IST8310_AVGCNTL) != 0x24) {
        bsp_error_handler(__FUNCTION__, __LINE__, "IST low noise mode failed.");
        return 0;
    }
    HAL_Delay(10);
    // Set/Reset pulse duration setup, normal mode
    ist8310_write_reg(IST8310_PDCNTL, 0xC0);
    if(ist8310_read_reg(IST8310_PDCNTL) != 0xC0) {
        bsp_error_handler(__FUNCTION__, __LINE__, "IST pulse duration set failed.");
        return 0;
    }
    HAL_Delay(10);
    // Turn off slave1 & slave 4
    mpu6500_write_reg(MPU6500_I2C_SLV1_CTRL, 0x00);
    HAL_Delay(10);
    mpu6500_write_reg(MPU6500_I2C_SLV4_CTRL, 0x00);
    HAL_Delay(10);
    // Configure and turn on slave 0
    mpu_read_ist_config(IST8310_ADDRESS, IST8310_R_XL, 0x06);
    HAL_Delay(100);
    return 1;
}

static uint8_t ist8310_read_reg(uint8_t addr) {
    mpu6500_write_reg(MPU6500_I2C_SLV4_REG, addr);
    HAL_Delay(10);
    mpu6500_write_reg(MPU6500_I2C_SLV4_CTRL, 0x80);
    HAL_Delay(10);
    uint8_t data = mpu6500_read_reg(MPU6500_I2C_SLV4_DI);
    mpu6500_write_reg(MPU6500_I2C_SLV4_CTRL, 0x00);     // Turn off slave4 after read
    HAL_Delay(10);
    return data;
}

static uint8_t ist8310_write_reg(uint8_t addr, uint8_t data) {
    mpu6500_write_reg(MPU6500_I2C_SLV1_CTRL, 0x00);     // Turn off slave 1 at first
    HAL_Delay(2);
    mpu6500_write_reg(MPU6500_I2C_SLV1_REG, addr);
    HAL_Delay(2);
    mpu6500_write_reg(MPU6500_I2C_SLV1_DO, data);
    HAL_Delay(2);
    // Turn on slave 1 with one byte transmitting
    mpu6500_write_reg(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
    HAL_Delay(10);
    return 1;
}

static void mpu_read_ist_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num) {
    // Use slave 1 to automatically transmit single measure mode
    mpu6500_write_reg(MPU6500_I2C_SLV1_ADDR, device_address);
    HAL_Delay(2);
    mpu6500_write_reg(MPU6500_I2C_SLV1_REG, IST8310_R_CONFA);
    HAL_Delay(2);
    mpu6500_write_reg(MPU6500_I2C_SLV1_DO, IST8310_ODR_MODE);
    HAL_Delay(2);
    // Use slave 0 to read data automatically
    mpu6500_write_reg(MPU6500_I2C_SLV0_ADDR, 0x80 | device_address);
    HAL_Delay(2);
    mpu6500_write_reg(MPU6500_I2C_SLV0_REG, reg_base_addr);
    HAL_Delay(2);
    // Every 8 mpu6500 internal samples, one i2c master read
    mpu6500_write_reg(MPU6500_I2C_SLV4_CTRL, 0x03);
    HAL_Delay(2);
    // Enable slave 0 and 1 access delay
    mpu6500_write_reg(MPU6500_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
    HAL_Delay(2);
    // Enable slave 1 auto transmit
    mpu6500_write_reg(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
    HAL_Delay(6); // Wait 6ms (minimum waiting time for 16 times internal average setup)
    // Enable slave 0 with data_num bytes reading
    mpu6500_write_reg(MPU6500_I2C_SLV0_CTRL, 0x80 | data_num);
    HAL_Delay(2);
}
