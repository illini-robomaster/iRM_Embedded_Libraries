/******************************************************************************
	Description
		Onboard IMU Utility
        This is a copy-paste of official reference program
	Log
        12/06/17 Nickel Liang	First Draft
        12/28/17 Nickel Liang   IMU structure change
*******************************************************************************/

#ifndef __IMU_ONBOARD
#define __IMU_ONBOARD

#include "mytype.h"
#include "math.h"

/* IMU constants. Should be customized for every board */
#define dt                0.02
#define q_angle_x         0.000002 // angle noise covariance cov(angle, angle)
#define q_gyro_x          0.0065 // gyro noise covariance cov(gyro, gyro) TODO: use static data from y and z axis for more accurate result.
#define r_angle           0.1 // measurement noise covariance
#define _STABLE_TEMP      27.6
#define _TEMP_THRESHOLD   0.2
#define _MAX_CALI_SAMPLES 1500 //1500 samples take approx 30 seconds.

/* IMU bias init value. Measured on new robot */
#define x_bias -1.270  //pitch
#define y_bias 0.63    //roll
#define z_bias -0.1682 //yaw

/* IMU Init calibration constants */
#define STATIC_YAW_THRESHOLD 4 //if gyro reading is greater than this, stop.

/* Register Table of IST8310 */
typedef enum {
    IST8310_WHO_AM_I           = 0x00,
    IST8310_ODR_MODE           = 0x01,
    IST8310_R_MODE             = 0x02,
    IST8310_R_XL               = 0x03,
    IST8310_R_XM               = 0x04,
    IST8310_R_YL               = 0x05,
    IST8310_R_YM               = 0x06,
    IST8310_R_ZL               = 0x07,
    IST8310_R_ZM               = 0x08,
    IST8310_R_CONFA            = 0x0A,
    IST8310_R_CONFB            = 0x0B,
    IST8310_ADDRESS            = 0x0E,
    IST8310_DEVICE_ID_A        = 0x10,
    IST8310_AVGCNTL            = 0x41,
    IST8310_PDCNTL             = 0x42,
} RM_IST8310_REG;

/* Register Table of MPU6500 */
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

    MPU6500_ID				   = 0x71,		// mpu6500 id = 0x71

    MPU_IIC_ADDR			   = 0x68,
} RM_MPU6500_REG;

/* Rx Data Structure */
typedef struct {
    struct {
        int16_t rx; // Raw data
        int16_t ry;
        int16_t rz;
        float x; // Decoded data
        float y;
        float z;
        int16_t x_offset; // Offset
        int16_t y_offset;
        int16_t z_offset;
    } acce;

    int16_t rtemp;
    float temp;

    struct {
        int16_t rx;
        int16_t ry;
        int16_t rz;
        float x;
        float y;
        float z;
        int16_t x_offset; // Offset
        int16_t y_offset;
        int16_t z_offset;
    } gyro;

    struct {
        int16_t rx;
        int16_t ry;
        int16_t rz;
        float x;
        float y;
        float z;
    } mag;

    struct {
        float x; //pitch (to the left of DJI icon)
        float y; //roll
        float z; //yaw
        float acc_x; //R to the left (roll)
        float acc_y; //R to the lower (pitch)
        float acc_z; //Z
        float init_acc_x; //Initial angles based on accelerometer readings
        float init_acc_y;
        float init_acc_z;
    } corrected_angle_data;

    struct {
        float pitch_bias;
        float roll_bias;
        float yaw_bias;
        int sampling_count;
    } calibration;

    uint8_t mpu_id;
    uint8_t mag_id;
} imu_t;

/* Declaration */
extern imu_t imuBoard;

/* Global Functions */
uint8_t RM_IMU_Onboard_Init(void);
uint8_t RM_IMU_MPU6500_Init(void);
uint8_t RM_IMU_IST8310_Init(void);
void RM_IMU_Onboard_GetData(void);
void RM_IMU_Temperature_Comp_init(void);
void RM_IMU_Temperature_Heating(void);
void RM_IMU_Temperature_Comp(void);
void RM_IMU_MPU6500_GetData(void);
void RM_IMU_IST8310_GetData(void);
void RM_MPU6500_ANGLE_UPDATE(void);


/* Static Functions */
static void RM_MPU6500_NSS_Low(void);
static void RM_MPU6500_DISCRETE_INTEGRAL(const float gyro_k, float *angle_storage, float cor_bias);
static void RM_MPU6500_NSS_High(void);
static void RM_MPU6500_UPDATE_ACC_ANGLE(void);
static void RM_MPU6500_KALMAN_FILTER_UPDATE(const float gyro_k,
        const float acc_angle, float *a_angle, double p_k[2][2], float * my_bias);
static void GET_ACC_STATIC_ANGLE(float grav_x, float grav_y, float grav_z, float *ret_x, float *ret_y, float *ret_z);
static uint8_t RM_MPU6500_Write_Reg(uint8_t const reg, uint8_t const data);
static uint8_t RM_MPU6500_Read_Reg(uint8_t const reg);
static uint8_t RM_MPU6500_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len);
static uint8_t RM_IST8310_Write_Reg(uint8_t addr, uint8_t data);
static uint8_t RM_IST8310_Read_Reg(uint8_t addr);
static void RM_MPU_Read_IST_Config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);

/* Debug Utility Functions */
void RM_DEBUG_IMU_PrintRaw(void);
void RM_DEBUG_IMU_PrintDecoded(void);
void RM_DEBUG_IMU_PrintKalman(void);

#endif
