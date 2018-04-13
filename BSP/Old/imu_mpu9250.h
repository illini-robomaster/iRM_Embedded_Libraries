// /******************************************************************************
// 	Description
// 		Seperate MPU9250 IMU Utility
// 	Log
//         02/01/17 Nickel Liang	First Draft
// *******************************************************************************/
//
// #ifndef __IMU_MPU9250
// #define __IMU_MPU9250
//
// #include "imu_onboard.h"
// #include "mytype.h"
// #include "math.h"
//
//
// /* Register Table of MPU9250 */
// typedef enum {
//     MPU9250_SELF_TEST_XG       = 0x00,
//     MPU9250_SELF_TEST_YG       = 0x01,
//     MPU9250_SELF_TEST_ZG       = 0x02,
//     MPU9250_SELF_TEST_XA       = 0x0D,
//     MPU9250_SELF_TEST_YA       = 0x0E,
//     MPU9250_SELF_TEST_ZA       = 0x0F,
//     MPU9250_XG_OFFSET_H        = 0x13,
//     MPU9250_XG_OFFSET_L        = 0x14,
//     MPU9250_YG_OFFSET_H        = 0x15,
//     MPU9250_YG_OFFSET_L        = 0x16,
//     MPU9250_ZG_OFFSET_H        = 0x17,
//     MPU9250_ZG_OFFSET_L        = 0x18,
//     MPU9250_SMPLRT_DIV         = 0x19,
//     MPU9250_CONFIG             = 0x1A,
//     MPU9250_GYRO_CONFIG        = 0x1B,
//     MPU9250_ACCEL_CONFIG       = 0x1C,
//     MPU9250_ACCEL_CONFIG_2     = 0x1D,
//     MPU9250_LP_ACCEL_ODR       = 0x1E,
//     MPU9250_WOM_THR            = 0x1F,
//     MPU9250_FIFO_EN            = 0x23,
//     MPU9250_I2C_MST_CTRL       = 0x24,
//     MPU9250_I2C_SLV0_ADDR      = 0x25,
//     MPU9250_I2C_SLV0_REG       = 0x26,
//     MPU9250_I2C_SLV0_CTRL      = 0x27,
//     MPU9250_I2C_SLV1_ADDR      = 0x28,
//     MPU9250_I2C_SLV1_REG       = 0x29,
//     MPU9250_I2C_SLV1_CTRL      = 0x2A,
//     MPU9250_I2C_SLV2_ADDR      = 0x2B,
//     MPU9250_I2C_SLV2_REG       = 0x2C,
//     MPU9250_I2C_SLV2_CTRL      = 0x2D,
//     MPU9250_I2C_SLV3_ADDR      = 0x2E,
//     MPU9250_I2C_SLV3_REG       = 0x2F,
//     MPU9250_I2C_SLV3_CTRL      = 0x30,
//     MPU9250_I2C_SLV4_ADDR      = 0x31,
//     MPU9250_I2C_SLV4_REG       = 0x32,
//     MPU9250_I2C_SLV4_DO        = 0x33,
//     MPU9250_I2C_SLV4_CTRL      = 0x34,
//     MPU9250_I2C_SLV4_DI        = 0x35,
//     MPU9250_I2C_MST_STATUS     = 0x36,
//     MPU9250_INT_PIN_CFG        = 0x37,
//     MPU9250_INT_ENABLE         = 0x38,
//     MPU9250_INT_STATUS         = 0x3A,
//     MPU9250_ACCEL_XOUT_H       = 0x3B,
//     MPU9250_ACCEL_XOUT_L       = 0x3C,
//     MPU9250_ACCEL_YOUT_H       = 0x3D,
//     MPU9250_ACCEL_YOUT_L       = 0x3E,
//     MPU9250_ACCEL_ZOUT_H       = 0x3F,
//     MPU9250_ACCEL_ZOUT_L       = 0x40,
//     MPU9250_TEMP_OUT_H         = 0x41,
//     MPU9250_TEMP_OUT_L         = 0x42,
//     MPU9250_GYRO_XOUT_H        = 0x43,
//     MPU9250_GYRO_XOUT_L        = 0x44,
//     MPU9250_GYRO_YOUT_H        = 0x45,
//     MPU9250_GYRO_YOUT_L        = 0x46,
//     MPU9250_GYRO_ZOUT_H        = 0x47,
//     MPU9250_GYRO_ZOUT_L        = 0x48,
//     MPU9250_EXT_SENS_DATA_00   = 0x49,
//     MPU9250_EXT_SENS_DATA_01   = 0x4A,
//     MPU9250_EXT_SENS_DATA_02   = 0x4B,
//     MPU9250_EXT_SENS_DATA_03   = 0x4C,
//     MPU9250_EXT_SENS_DATA_04   = 0x4D,
//     MPU9250_EXT_SENS_DATA_05   = 0x4E,
//     MPU9250_EXT_SENS_DATA_06   = 0x4F,
//     MPU9250_EXT_SENS_DATA_07   = 0x50,
//     MPU9250_EXT_SENS_DATA_08   = 0x51,
//     MPU9250_EXT_SENS_DATA_09   = 0x52,
//     MPU9250_EXT_SENS_DATA_10   = 0x53,
//     MPU9250_EXT_SENS_DATA_11   = 0x54,
//     MPU9250_EXT_SENS_DATA_12   = 0x55,
//     MPU9250_EXT_SENS_DATA_13   = 0x56,
//     MPU9250_EXT_SENS_DATA_14   = 0x57,
//     MPU9250_EXT_SENS_DATA_15   = 0x58,
//     MPU9250_EXT_SENS_DATA_16   = 0x59,
//     MPU9250_EXT_SENS_DATA_17   = 0x5A,
//     MPU9250_EXT_SENS_DATA_18   = 0x5B,
//     MPU9250_EXT_SENS_DATA_19   = 0x5C,
//     MPU9250_EXT_SENS_DATA_20   = 0x5D,
//     MPU9250_EXT_SENS_DATA_21   = 0x5E,
//     MPU9250_EXT_SENS_DATA_22   = 0x5F,
//     MPU9250_EXT_SENS_DATA_23   = 0x60,
//     MPU9250_I2C_SLV0_DO        = 0x63,
//     MPU9250_I2C_SLV1_DO        = 0x64,
//     MPU9250_I2C_SLV2_DO        = 0x65,
//     MPU9250_I2C_SLV3_DO        = 0x66,
//     MPU9250_I2C_MST_DELAY_CTRL = 0x67,
//     MPU9250_SIGNAL_PATH_RESET  = 0x68,
//     MPU9250_MOT_DETECT_CTRL    = 0x69,
//     MPU9250_USER_CTRL          = 0x6A,
//     MPU9250_PWR_MGMT_1         = 0x6B,
//     MPU9250_PWR_MGMT_2         = 0x6C,
//     MPU9250_FIFO_COUNTH        = 0x72,
//     MPU9250_FIFO_COUNTL        = 0x73,
//     MPU9250_FIFO_R_W           = 0x74,
//     MPU9250_WHO_AM_I           = 0x75,
//     MPU9250_XA_OFFSET_H        = 0x77,
//     MPU9250_XA_OFFSET_L        = 0x78,
//     MPU9250_YA_OFFSET_H        = 0x7A,
//     MPU9250_YA_OFFSET_L        = 0x7B,
//     MPU9250_ZA_OFFSET_H        = 0x7D,
//     MPU9250_ZA_OFFSET_L        = 0x7E,
//
//     MPU9250_ID				   = 0x71,
// } RM_MPU9250_REG;
//
// /* Register Table of MPU9250 Magnetometer AK8963 */
// typedef enum {
//     AK8963_WIA                 = 0x00, // Device ID 0x48
//     AK8963_INFO                = 0x01,
//     AK8963_ST1                 = 0x02,
//     AK8963_HXL                 = 0x03,
//     AK8963_HXH                 = 0x04,
//     AK8963_HYL                 = 0x05,
//     AK8963_HYH                 = 0x06,
//     AK8963_HZL                 = 0x07,
//     AK8963_HZH                 = 0x08,
//     AK8963_ST2                 = 0x09,
//     AK8963_CNTL                = 0x0A,
//     /*  "B0000": Power-down mode
//         "B0001": Single measurement mode
//         "B0010": Continuous measurement mode 1
//         "B0110": Continuous measurement mode 2
//         "B0100": External trigger measurement mode
//         "B1000": Self-test mode
//         "B1111": Fuse ROM access mode
//         B:  "0": 14-bit output
//             "1": 16-bit output  */
//     AK8963_RSV                 = 0x0B, // DO NOT USE
//     AK8963_ASTC                = 0x0C,
//     AK8963_TS1                 = 0x0D, // DO NOT USE
//     AK8963_TS2                 = 0x0E, // DO NOT USE
//     AK8963_I2CDIS              = 0x0F,
//     AK8963_ASAX                = 0x10,
//     AK8963_ASAY                = 0x11,
//     AK8963_ASAZ                = 0x12,
// } RM_AK8963_REG;
//
//
// /* Rx Data Structure */
// /* This structure definition is in imu_onboard.h */
//
// /* Declaration */
// extern imu_t imuChassis;
// // extern imu_t imuShoot;
// // extern imu_t imuCamera;
//
// /* Global Functions */
// uint8_t RM_IMU_MPU9250_Init(void);
// void RM_IMU_MPU9250_GetData(void);
// // void RM_MPU9250_ANGLE_UPDATE(void);
// // uint8_t RM_IMU_IST8310_Init(void);
//
// /* Static Functions */
// static void RM_MPU9250_NSS_Low(void);
// // static void RM_MPU9250_DISCRETE_INTEGRAL(const float gyro_k, float *angle_storage);
// static void RM_MPU9250_NSS_High(void);
// // static void RM_MPU9250_UPDATE_ACC_ANGLE(void);
// // static void RM_MPU9250_KALMAN_FILTER_UPDATE(const float gyro_k, const float acc_angle, float *a_angle, double p_k[2][2]);
// // static void GET_ACC_STATIC_ANGLE(float grav_x, float grav_y, float grav_z, float *ret_x, float *ret_y, float *ret_z);
// static uint8_t RM_MPU9250_Write_Reg(uint8_t const reg, uint8_t const data);
// static uint8_t RM_MPU9250_Read_Reg(uint8_t const reg);
// static uint8_t RM_MPU9250_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len);
// // static uint8_t RM_IST8310_Write_Reg(uint8_t addr, uint8_t data);
// // static uint8_t RM_IST8310_Read_Reg(uint8_t addr);
// // static void RM_MPU_Read_IST_Init(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);
// // static uint8_t RM_MPU9250_Set_Accel_Fsr(uint8_t fsr);
// // static uint8_t RM_MPU9250_Set_Gyro_Fsr(uint8_t fsr);
//
// /* Debug Utility Functions */
// void RM_DEBUG_IMU9250_PrintRaw(void);
// void RM_DEBUG_IMU9250_PrintDecoded(void);
// void RM_DEBUG_IMU9250_PrintKalman(void);
//
// #endif
