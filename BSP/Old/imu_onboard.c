/******************************************************************************
	Description
		Onboard IMU Utility
    Log
        12/06/17 Nickel Liang   First Draft. Direct copy from sample program.
        12/07/17 Nickel Liang   Functionality Improvements & Dubug
        12/12/17 Roger Qiu      Kalman filter first draft
        12/28/17 Nickel Liang   Bug fix & decode
        03/23/18 Roger Qiu      Finished basic functions; now it's working
*******************************************************************************/

#include "imu_onboard.h"

/* Global Var */
imu_t imuBoard;
double p_k_x[2][2] = {{0, 0}, {0, 0}}; //covariance matrix
double p_k_y[2][2] = {{0, 0}, {0, 0}};
double p_k_z[2][2] = {{0, 0}, {0, 0}};

float debug_output = 0;

/* Local Var */
int calibration_flag = 1;

void RM_IMU_Onboard_GetData(void) {
    RM_IMU_MPU6500_GetData();
    RM_IMU_IST8310_GetData();
}

void RM_IMU_Temperature_Comp_init(void){
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    //TIM3->PSC = 83;
}

void RM_IMU_Temperature_Heating(void){
    //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 500);
    TIM3->ARR = 1000000; //WTF
    TIM3->CCR2 = 950000;
}

void RM_IMU_Temperature_Comp(void){
    if(_STABLE_TEMP - imuBoard.temp > _TEMP_THRESHOLD){
        RM_IMU_Temperature_Heating();
    } else {
        TIM3->CCR2 = 0;
    }
}

uint8_t RM_IMU_Onboard_Init(void) {
    // Init MPU6500 & IST8310
    RM_IMU_MPU6500_Init();
    RM_IMU_Temperature_Comp_init();
    RM_IMU_IST8310_Init();
    RM_IMU_Onboard_GetData();
    imuBoard.calibration.pitch_bias = x_bias;
    imuBoard.calibration.roll_bias = y_bias;
    imuBoard.calibration.yaw_bias = z_bias;
    imuBoard.calibration.sampling_count = 0;
    RM_MPU6500_UPDATE_ACC_ANGLE();
    imuBoard.corrected_angle_data.init_acc_x = imuBoard.corrected_angle_data.acc_x;
    imuBoard.corrected_angle_data.init_acc_y = imuBoard.corrected_angle_data.acc_y;
    imuBoard.corrected_angle_data.init_acc_z = imuBoard.corrected_angle_data.acc_z;
    imuBoard.corrected_angle_data.x = imuBoard.corrected_angle_data.init_acc_x;
    imuBoard.corrected_angle_data.y = imuBoard.corrected_angle_data.init_acc_y;
    imuBoard.corrected_angle_data.z = imuBoard.corrected_angle_data.init_acc_z;
    return 0;
}

/******************************************************************************
	Input
	Output
	Description
		Get 6 Axis Data from MPU 6500
	Log
		12/07/17 Nickel Liang	First Draft
        12/27/17 Nickel Liang   Move mpu_buff
        12/28/17 Nickel Liang   Decode
*******************************************************************************/
void RM_IMU_MPU6500_GetData(void) {
    uint8_t mpu_buff[14];
    // Must read 14 all together
    RM_MPU6500_Read_Regs(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);

    float acceFactor = 4096.0; // Check datasheet, 8g = 4096
    imuBoard.acce.rx = (int16_t)(mpu_buff[0]<<8 | mpu_buff[1]);
    imuBoard.acce.ry = (int16_t)(mpu_buff[2]<<8 | mpu_buff[3]);
    imuBoard.acce.rz = (int16_t)(mpu_buff[4]<<8 | mpu_buff[5]);
    imuBoard.acce.x = (float)(imuBoard.acce.rx/acceFactor);
    imuBoard.acce.y = (float)(imuBoard.acce.ry/acceFactor);
    imuBoard.acce.z = (float)(imuBoard.acce.rz/acceFactor);

    float roomTempOffset = 0; // 21 C
    float tempSensitivity = 333.87; // Datasheet p12
    imuBoard.rtemp = (int16_t)(mpu_buff[6]<<8 | mpu_buff[7]);
    imuBoard.temp = (float)(((imuBoard.rtemp-roomTempOffset)/tempSensitivity) + 21); // Register map p33

    float gyroFactor = 16.4; // Check datasheet, 2000dps = 16.4
    imuBoard.gyro.rx = (int16_t)(mpu_buff[8]<<8 | mpu_buff[9]);
    imuBoard.gyro.ry = (int16_t)(mpu_buff[10]<<8 | mpu_buff[11]);
    imuBoard.gyro.rz = (int16_t)(mpu_buff[12]<<8 | mpu_buff[13]);
    imuBoard.gyro.x = (float)(imuBoard.gyro.rx/gyroFactor); // Get a value in degree
    imuBoard.gyro.y = (float)(imuBoard.gyro.ry/gyroFactor);
    imuBoard.gyro.z = (float)(imuBoard.gyro.rz/gyroFactor);
}

void RM_IMU_IST8310_GetData(void) {
    uint8_t ist_buff[6];
    RM_MPU6500_Read_Regs(MPU6500_EXT_SENS_DATA_00, ist_buff, 6);
    imuBoard.mag.rx = (int16_t)(ist_buff[1]<<8 | ist_buff[0]);
    imuBoard.mag.ry = (int16_t)(ist_buff[3]<<8 | ist_buff[2]);
    imuBoard.mag.rz = (int16_t)(ist_buff[5]<<8 | ist_buff[4]);
}

/******************************************************************************
	Input
	Output
	Description
		Need to be regularly called to maintain the accessible data
	Log
		01/12/18 Roger Qiu	First Draft
*******************************************************************************/
void RM_MPU6500_ANGLE_UPDATE(void){
    RM_IMU_MPU6500_GetData();
    RM_MPU6500_UPDATE_ACC_ANGLE();
    if(calibration_flag == 1){
        float pitch_orig_sum = imuBoard.calibration.sampling_count * imuBoard.calibration.yaw_bias;
        float roll_orig_sum = imuBoard.calibration.sampling_count * imuBoard.calibration.yaw_bias;
        float yaw_orig_sum = imuBoard.calibration.sampling_count * imuBoard.calibration.yaw_bias;
        if(abs(imuBoard.gyro.z) > STATIC_YAW_THRESHOLD){
            //IMU is moved!!!
            calibration_flag = 0;
        } else {
            //suppose it's static
            float new_pitch_sum = pitch_orig_sum + imuBoard.gyro.x;
            float new_roll_sum = roll_orig_sum + imuBoard.gyro.y;
            float new_yaw_sum = yaw_orig_sum + imuBoard.gyro.z;
            imuBoard.calibration.pitch_bias = new_pitch_sum / (imuBoard.calibration.sampling_count + 1);
            imuBoard.calibration.roll_bias = new_roll_sum / (imuBoard.calibration.sampling_count + 1);
            imuBoard.calibration.yaw_bias = new_yaw_sum / (imuBoard.calibration.sampling_count + 1);
            imuBoard.calibration.sampling_count++;
            imuBoard.corrected_angle_data.z = 0; //since we assume it's static.
            if(imuBoard.calibration.sampling_count > _MAX_CALI_SAMPLES){
                //enough sampling. Cease!
                calibration_flag = 0;
            }
        }
    }
    RM_MPU6500_KALMAN_FILTER_UPDATE(imuBoard.gyro.x,
        imuBoard.corrected_angle_data.acc_x - imuBoard.corrected_angle_data.init_acc_x,
        &imuBoard.corrected_angle_data.x, p_k_x, &(imuBoard.calibration.pitch_bias));
    RM_MPU6500_KALMAN_FILTER_UPDATE(imuBoard.gyro.y,
        imuBoard.corrected_angle_data.acc_y - imuBoard.corrected_angle_data.init_acc_y,
        &imuBoard.corrected_angle_data.y, p_k_y, &(imuBoard.calibration.roll_bias));
    RM_MPU6500_DISCRETE_INTEGRAL(imuBoard.gyro.x, &(debug_output),
        imuBoard.calibration.pitch_bias);
    RM_MPU6500_DISCRETE_INTEGRAL(imuBoard.gyro.z, &imuBoard.corrected_angle_data.z,
        imuBoard.calibration.yaw_bias);
    RM_IMU_Temperature_Comp();
}

/******************************************************************************
	Input
	Output
	Description
		Debug utility function
	Log
		12/27/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_DEBUG_IMU_PrintRaw(void) {
    RM_IMU_Onboard_GetData();
    RM_PRINTF("[  RAW  ] ID %02x | ", imuBoard.mpu_id);
    RM_PRINTF("Acce X %d \tY %d \tZ %d \t| ", imuBoard.acce.rx, imuBoard.acce.ry, imuBoard.acce.rz);
    RM_PRINTF("Gyro X %d \tY %d \tZ %d \t| ", imuBoard.gyro.rx, imuBoard.gyro.ry, imuBoard.gyro.rz);
    RM_PRINTF("Magn X %d \tY %d \tZ %d \t| ", imuBoard.mag.rx,  imuBoard.mag.ry,  imuBoard.mag.rz);
    RM_PRINTF("Temp %d\r\n", imuBoard.rtemp);
}

/******************************************************************************
	Input
	Output
	Description
		Debug utility function
	Log
		12/27/17 Nickel Liang	First Draft
*******************************************************************************/
void RM_DEBUG_IMU_PrintDecoded(void) {
    RM_IMU_Onboard_GetData();
    RM_PRINTF("[DECODED] ID %02x | ", imuBoard.mpu_id);
    RM_PRINTF("Acce X %f \tY %f \tZ %f \t| ", imuBoard.acce.x, imuBoard.acce.y, imuBoard.acce.z);
    RM_PRINTF("Gyro X %f \tY %f \tZ %f \t| ", imuBoard.gyro.x, imuBoard.gyro.y, imuBoard.gyro.z);
    RM_PRINTF("Temp %f\r\n", imuBoard.temp);
}

void RM_DEBUG_IMU_PrintKalman(void){
    RM_PRINTF("[Filtered] ID %02x | ", imuBoard.mpu_id);
    RM_PRINTF("Kalman_Gyro X %f \tY %f \tZ %f \t| ", (double)(imuBoard.corrected_angle_data.x), imuBoard.corrected_angle_data.y, imuBoard.corrected_angle_data.z);
    RM_PRINTF("Acce X %f \tY %f \tZ %f \t| ", imuBoard.acce.x, imuBoard.acce.y, imuBoard.acce.z);
    RM_PRINTF("Gyro X %f \tY %f \tZ %f \t| ", imuBoard.gyro.x, imuBoard.gyro.y, imuBoard.gyro.z);
    RM_PRINTF("Acc_angle X %f \tY %f \tZ %f \t| ", imuBoard.corrected_angle_data.acc_x, imuBoard.corrected_angle_data.acc_y, imuBoard.corrected_angle_data.acc_z);
    RM_PRINTF("Debug X %f \tY %f \tZ %f \t| ", debug_output, debug_output, debug_output);
    RM_PRINTF("Temp IMU %f |\r\n", imuBoard.temp);
}

/******************************************************************************
    Input
	Output
	Description
		Update accessible acce data
	Log
		01/12/18 Roger Qiu	First Draft
        01/14/18 Roger Qiu Bug fix
*******************************************************************************/
static void RM_MPU6500_UPDATE_ACC_ANGLE(void) {
    GET_ACC_STATIC_ANGLE(imuBoard.acce.x, imuBoard.acce.y, imuBoard.acce.z, &imuBoard.corrected_angle_data.acc_x, &imuBoard.corrected_angle_data.acc_y, &imuBoard.corrected_angle_data.acc_z);
    //imuBoard.corrected_angle_data.acc_z = 0; //TODO: change to mag
}

/******************************************************************************
	Input
        - accelerometer Readings
	Output
	Description
		Calculate angles from accelerometer readings
	Log
		01/12/18 Roger Qiu	First Draft
*******************************************************************************/
static void GET_ACC_STATIC_ANGLE(float grav_x, float grav_y, float grav_z, float *ret_x, float *ret_y, float *ret_z){
    *ret_x = atan2(grav_y, grav_z) * rad_2_deg; //roll
    *ret_y = atan2(grav_x, grav_z) * rad_2_deg; //pitch
    float grav_scalar = sqrt(grav_x * grav_x + grav_y * grav_y + grav_z * grav_z);
    //*ret_x = acos(grav_x / grav_scalar) * rad_2_deg;
    //*ret_y = acos(grav_y / grav_scalar) * rad_2_deg;
    *ret_z = acos(grav_z / grav_scalar) * rad_2_deg;
}

//should only be used on yaw
static void RM_MPU6500_DISCRETE_INTEGRAL(const float gyro_k, float *angle_holder, float cor_bias){
    float rate = gyro_k - cor_bias;
    *angle_holder = *angle_holder + dt * rate;
}

/******************************************************************************
	Input
        - gyro_k: angular velocity from gyro.
        - acc_angle: angle measured by accelerometer
        - *a_angle: pointer to a axis
        - *p_k: pointer to covariance matrix of an axis
	Description
		Calculate Kalman filter adjusted angle
	Log
		12/12/17 Roger Qiu	First Draft
        01/12/18 Roger Qiu Bug Fix; parameter updated; it's now working!
*******************************************************************************/
static void RM_MPU6500_KALMAN_FILTER_UPDATE(const float gyro_k, const float acc_angle,
        float *a_angle, double p_k[2][2], float * my_bias){ //imuboard.gyro.x, acc_angle, desired_ang
    float rate = gyro_k - *(my_bias);
    float p_cache[4] = {0, 0, 0, 0};
    //EQ 1 - A priori state estimate
    //ideal estimated angle
    *a_angle += rate * dt; //a priori estimate
    //EQ 2 - A priori estimate covariance
    //p_{k}_{k-1} (estimate from past observations)
    p_cache[0] = q_angle_x - p_k[0][1] - p_k[1][0] + dt * p_k[1][1];
    p_cache[1] = -1 * p_k[1][1];
    p_cache[2] = -1 * p_k[1][1];
    p_cache[3] = q_gyro_x;
    p_k[0][0] += p_cache[0] * dt; // dt^2 << 0. Ignore.
    p_k[0][1] += p_cache[1] * dt;
    p_k[1][0] += p_cache[2] * dt;
    p_k[1][1] += p_cache[3] * dt;
    //EQ 3 - Optimal Kalman Gain
    float angle_err = acc_angle - *a_angle;
    float E = r_angle + p_k[0][0];
    float k_0 = p_k[0][0] / E;
    float k_1 = p_k[1][0] / E;
    //EQ 4 (and 5) - A posteriori estimate covariance (update original covariance matrix)
    p_k[0][0] -= k_0 * p_k[0][0];
    p_k[0][1] -= k_0 * p_k[0][1];
    p_k[1][0] -= k_1 * p_k[0][0];
    p_k[1][1] -= k_1 * p_k[0][1];
    //calculate adjusted angle
    *a_angle += k_0 * angle_err;
    *(my_bias) += k_1 * angle_err;
}

/******************************************************************************
	Input
	Output
	Description
		Pull NSS Pin Low. Select MPU6500.
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static void RM_MPU6500_NSS_Low(void) {
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
}

/******************************************************************************
	Input
	Output
	Description
		Pull NSS Pin High. Invert Select MPU6500.
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static void RM_MPU6500_NSS_High(void) {
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
}

/******************************************************************************
	Input
        reg register to be written
        data data to be written
	Output
        0
	Description
		Write a register on MPU6500
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static uint8_t RM_MPU6500_Write_Reg(uint8_t const reg, uint8_t const data) {
    static uint8_t MPU_Rx, MPU_Tx;
    RM_MPU6500_NSS_Low();
    MPU_Tx = reg & 0x7f;
    HAL_SPI_TransmitReceive(&hspi5, &MPU_Tx, &MPU_Rx, 1, 55);
    MPU_Tx = data;
    HAL_SPI_TransmitReceive(&hspi5, &MPU_Tx, &MPU_Rx, 1, 55);
    RM_MPU6500_NSS_High();
    return 0;
}

/******************************************************************************
	Input
        reg register to be read
	Output
        Received data
	Description
		Read a register on MPU6500
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static uint8_t RM_MPU6500_Read_Reg(uint8_t const reg) {
    static uint8_t MPU_Rx, MPU_Tx;
    RM_MPU6500_NSS_Low();
    MPU_Tx = reg | 0x80;
    HAL_SPI_TransmitReceive(&hspi5, &MPU_Tx, &MPU_Rx, 1, 55);
    HAL_SPI_TransmitReceive(&hspi5, &MPU_Tx, &MPU_Rx, 1, 55);
    RM_MPU6500_NSS_High();
    return MPU_Rx;
}

/******************************************************************************
	Input
        regAddr start addr
        pData received data
        len how many consecutive registers
	Output
        0
	Description
		Read registers on MPU6500
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static uint8_t RM_MPU6500_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len) {
    static uint8_t MPU_Rx, MPU_Tx, MPU_Tx_buff[14] = {0xff};
    RM_MPU6500_NSS_Low();
    MPU_Tx = regAddr | 0x80;
    MPU_Tx_buff[0] = MPU_Tx;
    HAL_SPI_TransmitReceive(&hspi5, &MPU_Tx, &MPU_Rx, 1, 55);
    HAL_SPI_TransmitReceive(&hspi5, MPU_Tx_buff, pData, len, 55);
    RM_MPU6500_NSS_High();
    return 0;
}

/******************************************************************************
	Input
	Output
        0
	Description
		Initialize Onboard MPU6500 (Acceleration and Gyro)
	Log
		12/07/17 Nickel Liang	First Draft
        01/12/18 Roger Qiu      Add Angle initialization
*******************************************************************************/
uint8_t RM_IMU_MPU6500_Init(void) {
    // Reset the internal registers
    RM_MPU6500_Write_Reg(MPU6500_PWR_MGMT_1, 0x80);
    HAL_Delay(100);
    // Reset gyro/accel/temp digital signal path
    RM_MPU6500_Write_Reg(MPU6500_SIGNAL_PATH_RESET, 0x07);
    HAL_Delay(100);
    // read id of device,check if MPU6500 or not
    imuBoard.mpu_id = RM_MPU6500_Read_Reg(MPU6500_WHO_AM_I);

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
    for(index = 0; index < 10; index++) {
        RM_MPU6500_Write_Reg(MPU6500_Init_Data[index][0], MPU6500_Init_Data[index][1]);
        HAL_Delay(1);
    }
    return 0;
}

/******************************************************************************
	Input
	Output
        0 - Success
        Other - Error
	Description
		Initialize Onboard IST8310 Magnetic Sensor
	Log
		12/07/17 Nickel Liang	First Draft
        12/30/17 Nickel Liang   Comments and fix
*******************************************************************************/
uint8_t RM_IMU_IST8310_Init(void) {
    RM_MPU6500_Write_Reg(MPU6500_USER_CTRL, 0x30); // Enable the I2C Master I/F module, Reset I2C Slave module
    HAL_Delay(10);
    RM_MPU6500_Write_Reg(MPU6500_I2C_MST_CTRL, 0x0d); // I2C master clock 400kHz
    HAL_Delay(10);
    // turn on slave 1 for ist write and slave 4 for ist read
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_ADDR, IST8310_ADDRESS); // Write from slave 1
    HAL_Delay(10);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV4_ADDR, 0x80 | IST8310_ADDRESS); // Read from slave 4
    HAL_Delay(10);
    // reset ist8310
    RM_IST8310_Write_Reg(IST8310_R_CONFB, 0x01);
    HAL_Delay(10);
    imuBoard.mag_id = RM_IST8310_Read_Reg(IST8310_WHO_AM_I);
    if(IST8310_DEVICE_ID_A != RM_IST8310_Read_Reg(IST8310_WHO_AM_I))
        return 1; //error
    // reset ist8310
    RM_IST8310_Write_Reg(IST8310_R_CONFB, 0x01);
    HAL_Delay(10);
    // config as ready mode to access reg
    RM_IST8310_Write_Reg(IST8310_R_CONFA, 0x00);
    if(RM_IST8310_Read_Reg(IST8310_R_CONFA) != 0x00)
        return 2;
    HAL_Delay(10);
    // normal state, no int
    RM_IST8310_Write_Reg(IST8310_R_CONFB, 0x00);
    if(RM_IST8310_Read_Reg(IST8310_R_CONFB) != 0x00)
        return 3;
    HAL_Delay(10);
    // config low noise mode, x,y,z axis 16 time 1 avg,
    RM_IST8310_Write_Reg(IST8310_AVGCNTL, 0x24); // 100100
    if(RM_IST8310_Read_Reg(IST8310_AVGCNTL) != 0x24)
        return 4;
    HAL_Delay(10);
    // Set/Reset pulse duration setup, normal mode
    RM_IST8310_Write_Reg(IST8310_PDCNTL, 0xc0);
    if(RM_IST8310_Read_Reg(IST8310_PDCNTL) != 0xc0)
        return 5;
    HAL_Delay(10);
    // turn off slave1 & slave 4
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x00);
    HAL_Delay(10);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x00);
    HAL_Delay(10);
    // configure and turn on slave 0
    RM_MPU_Read_IST_Config(IST8310_ADDRESS, IST8310_R_XL, 0x06);
    HAL_Delay(100);
    return 0;
}

/******************************************************************************
	Input
        addr register to be written
        data data to be written
	Output
        0
	Description
		Write a IST8310 register through MPU6500
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static uint8_t RM_IST8310_Write_Reg(uint8_t addr, uint8_t data) {
    // turn off slave 1 at first
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x00);
    HAL_Delay(2);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_REG, addr);
    HAL_Delay(2);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_DO, data);
    HAL_Delay(2);
    // turn on slave 1 with one byte transmitting
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x080 | 0x01);
    HAL_Delay(10);
    return 0;
}

/******************************************************************************
	Input
        addr register to be written
	Output
        received data
	Description
		Read a IST8310 register through MPU6500
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static uint8_t RM_IST8310_Read_Reg(uint8_t addr) {
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV4_REG, addr);
    HAL_Delay(10);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x80);
    HAL_Delay(10);
    uint8_t data = RM_MPU6500_Read_Reg(MPU6500_I2C_SLV4_DI);
    // turn off slave4 after read
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x00);
    HAL_Delay(10);
    return data;
}

/******************************************************************************
	Input
        device_address
        reg_base_addr
        data_num number of data
	Output
	Description
		Configure MPU6500 to read data from IST8310
        Initialize the MPU6500 I2C Slave0 for I2C reading
	Log
		12/07/17 Nickel Liang	First Draft
*******************************************************************************/
static void RM_MPU_Read_IST_Config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num) {
    // configure the device address of the IST8310
    // use slave1,auto transmit single measure mode.
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_ADDR, device_address);
    HAL_Delay(2);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_REG, IST8310_R_CONFA);
    HAL_Delay(2);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_DO, IST8310_ODR_MODE);
    HAL_Delay(2);
    // use slave0,auto read data
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV0_ADDR, 0x80 | device_address);
    HAL_Delay(2);
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV0_REG, reg_base_addr);
    HAL_Delay(2);
    // every eight mpu6500 internal samples one i2c master read
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x03);
    HAL_Delay(2);
    // enable slave 0 and 1 access delay
    RM_MPU6500_Write_Reg(MPU6500_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
    HAL_Delay(2);
    // enable slave 1 auto transmit
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
    HAL_Delay(6);
    // enable slave 0 with data_num bytes reading
    RM_MPU6500_Write_Reg(MPU6500_I2C_SLV0_CTRL, 0x80 | data_num);
    HAL_Delay(2);
}
