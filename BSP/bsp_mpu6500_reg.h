/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-17
 * @file    bsp_mpu6500_reg.h
 * @brief   MPU6500 gyroscope register table
 * @log     2018-04-20 nickelliang
 */

#ifndef _BSP_MPU6500_REG_H_
#define _BSP_MPU6500_REG_H_

/**
 * @ingroup bsp
 * @defgroup bsp_ist6500_reg BSP IST6500 REG
 * @{
 */

typedef enum {
    MPU6500_SELF_TEST_XG       = 0x00,
    MPU6500_SELF_TEST_YG       = 0x01,
    MPU6500_SELF_TEST_ZG       = 0x02,
    MPU6500_SELF_TEST_XA       = 0x0D,
    MPU6500_SELF_TEST_YA       = 0x0E,
    MPU6500_SELF_TEST_ZA       = 0x0F,
    MPU6500_XG_OFFSET_H        = 0x13,
    MPU6500_XG_OFFSET_L        = 0x14,
    MPU6500_YG_OFFSET_H        = 0x15,
    MPU6500_YG_OFFSET_L        = 0x16,
    MPU6500_ZG_OFFSET_H        = 0x17,
    MPU6500_ZG_OFFSET_L        = 0x18,
    MPU6500_SMPLRT_DIV         = 0x19,
    MPU6500_CONFIG             = 0x1A,
    MPU6500_GYRO_CONFIG        = 0x1B,
    MPU6500_ACCEL_CONFIG       = 0x1C,
    MPU6500_ACCEL_CONFIG_2     = 0x1D,
    MPU6500_LP_ACCEL_ODR       = 0x1E,
    MPU6500_MOT_THR            = 0x1F,
    MPU6500_FIFO_EN            = 0x23,
    MPU6500_I2C_MST_CTRL       = 0x24,
    MPU6500_I2C_SLV0_ADDR      = 0x25,
    MPU6500_I2C_SLV0_REG       = 0x26,
    MPU6500_I2C_SLV0_CTRL      = 0x27,
    MPU6500_I2C_SLV1_ADDR      = 0x28,
    MPU6500_I2C_SLV1_REG       = 0x29,
    MPU6500_I2C_SLV1_CTRL      = 0x2A,
    MPU6500_I2C_SLV2_ADDR      = 0x2B,
    MPU6500_I2C_SLV2_REG       = 0x2C,
    MPU6500_I2C_SLV2_CTRL      = 0x2D,
    MPU6500_I2C_SLV3_ADDR      = 0x2E,
    MPU6500_I2C_SLV3_REG       = 0x2F,
    MPU6500_I2C_SLV3_CTRL      = 0x30,
    MPU6500_I2C_SLV4_ADDR      = 0x31,
    MPU6500_I2C_SLV4_REG       = 0x32,
    MPU6500_I2C_SLV4_DO        = 0x33,
    MPU6500_I2C_SLV4_CTRL      = 0x34,
    MPU6500_I2C_SLV4_DI        = 0x35,
    MPU6500_I2C_MST_STATUS     = 0x36,
    MPU6500_INT_PIN_CFG        = 0x37,
    MPU6500_INT_ENABLE         = 0x38,
    MPU6500_INT_STATUS         = 0x3A,
    MPU6500_ACCEL_XOUT_H       = 0x3B,
    MPU6500_ACCEL_XOUT_L       = 0x3C,
    MPU6500_ACCEL_YOUT_H       = 0x3D,
    MPU6500_ACCEL_YOUT_L       = 0x3E,
    MPU6500_ACCEL_ZOUT_H       = 0x3F,
    MPU6500_ACCEL_ZOUT_L       = 0x40,
    MPU6500_TEMP_OUT_H         = 0x41,
    MPU6500_TEMP_OUT_L         = 0x42,
    MPU6500_GYRO_XOUT_H        = 0x43,
    MPU6500_GYRO_XOUT_L        = 0x44,
    MPU6500_GYRO_YOUT_H        = 0x45,
    MPU6500_GYRO_YOUT_L        = 0x46,
    MPU6500_GYRO_ZOUT_H        = 0x47,
    MPU6500_GYRO_ZOUT_L        = 0x48,
    MPU6500_EXT_SENS_DATA_00   = 0x49,
    MPU6500_EXT_SENS_DATA_01   = 0x4A,
    MPU6500_EXT_SENS_DATA_02   = 0x4B,
    MPU6500_EXT_SENS_DATA_03   = 0x4C,
    MPU6500_EXT_SENS_DATA_04   = 0x4D,
    MPU6500_EXT_SENS_DATA_05   = 0x4E,
    MPU6500_EXT_SENS_DATA_06   = 0x4F,
    MPU6500_EXT_SENS_DATA_07   = 0x50,
    MPU6500_EXT_SENS_DATA_08   = 0x51,
    MPU6500_EXT_SENS_DATA_09   = 0x52,
    MPU6500_EXT_SENS_DATA_10   = 0x53,
    MPU6500_EXT_SENS_DATA_11   = 0x54,
    MPU6500_EXT_SENS_DATA_12   = 0x55,
    MPU6500_EXT_SENS_DATA_13   = 0x56,
    MPU6500_EXT_SENS_DATA_14   = 0x57,
    MPU6500_EXT_SENS_DATA_15   = 0x58,
    MPU6500_EXT_SENS_DATA_16   = 0x59,
    MPU6500_EXT_SENS_DATA_17   = 0x5A,
    MPU6500_EXT_SENS_DATA_18   = 0x5B,
    MPU6500_EXT_SENS_DATA_19   = 0x5C,
    MPU6500_EXT_SENS_DATA_20   = 0x5D,
    MPU6500_EXT_SENS_DATA_21   = 0x5E,
    MPU6500_EXT_SENS_DATA_22   = 0x5F,
    MPU6500_EXT_SENS_DATA_23   = 0x60,
    MPU6500_I2C_SLV0_DO        = 0x63,
    MPU6500_I2C_SLV1_DO        = 0x64,
    MPU6500_I2C_SLV2_DO        = 0x65,
    MPU6500_I2C_SLV3_DO        = 0x66,
    MPU6500_I2C_MST_DELAY_CTRL = 0x67,
    MPU6500_SIGNAL_PATH_RESET  = 0x68,
    MPU6500_MOT_DETECT_CTRL    = 0x69,
    MPU6500_USER_CTRL          = 0x6A,
    MPU6500_PWR_MGMT_1         = 0x6B,
    MPU6500_PWR_MGMT_2         = 0x6C,
    MPU6500_FIFO_COUNTH        = 0x72,
    MPU6500_FIFO_COUNTL        = 0x73,
    MPU6500_FIFO_R_W           = 0x74,
    MPU6500_WHO_AM_I           = 0x75,
    MPU6500_XA_OFFSET_H        = 0x77,
    MPU6500_XA_OFFSET_L        = 0x78,
    MPU6500_YA_OFFSET_H        = 0x7A,
    MPU6500_YA_OFFSET_L        = 0x7B,
    MPU6500_ZA_OFFSET_H        = 0x7D,
    MPU6500_ZA_OFFSET_L        = 0x7E,

    MPU6500_ID				   = 0x70,    // mpu6500 id = 0x70

    MPU_IIC_ADDR			   = 0x68,
} MPU6500_REG;

/** @} */

#endif
