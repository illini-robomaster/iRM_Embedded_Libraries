// /******************************************************************************
// 	Description
// 		Onboard IMU Utility
//     Log
//         12/06/17 Nickel Liang   First Draft. Direct copy from sample program.
//         12/07/17 Nickel Liang   Functionality Improvements & Dubug
//         12/12/17 Roger Qiu      Kalman filter first draft
//         12/28/17 Nickel Liang   Bug fix & decode
// *******************************************************************************/
//
// #include "imu_mpu9250.h"
//
// /* Global Var */
// imu_t imuChassis;
// // imu_t imuShoot;
// // imu_t imuCamera;
//
// // double p_k_x[2][2] = {{0, 0}, {0, 0}}; //covariance matrix
// // double p_k_y[2][2] = {{0, 0}, {0, 0}};
// // double p_k_z[2][2] = {{0, 0}, {0, 0}};
// //float q_bias = 0.0; // gyro bias
// //float a_angle_x = 0.0; // current angle
//
// /* Local Var */
// // static float dt = 0.023; // sampling interval
// // float q_bias = -0.985; // gyro bias
// // float y_bias = -1.190;
// // float z_bias = -0.007; //yaw bias
// // static float q_angle_x = 0.000002; // angle noise covariance cov(angle, angle)
// // static float q_gyro_x = 0.0065; // gyro noise covariance cov(gyro, gyro) TODO: use static data from y and z axis for more accurate result.
// // static float r_angle = 0.1; // measurement noise covariance
// // const float pi = 3.14159;
// // const float rad_2_deg = 360 / 6.28;
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Initialize Onboard MPU9250 (Acceleration and Gyro)
// 	Log
// 		02/01/18 Nickel Liang	First Draft
// *******************************************************************************/
// uint8_t RM_IMU_MPU9250_Init(void) {
//     uint8_t index = 0;
//     uint8_t MPU9250_Init_Data[10][2] = {
//         {MPU9250_PWR_MGMT_1,     0x80},      // Reset Device
//         {MPU9250_PWR_MGMT_1,     0x03},      // Clock Source - Gyro-Z
//         {MPU9250_PWR_MGMT_2,     0x00},      // Enable Acc & Gyro
//         {MPU9250_CONFIG,         0x02},      // LPF 92Hz
//         {MPU9250_GYRO_CONFIG,    0x18},      // 0x00 = 250dps / factor 131, 0x08 = 500dps / 65.5, 0x10 = 1000dps / 32.8, 0x18 = 2000dps / 16.4
//         {MPU9250_ACCEL_CONFIG,   0x10},      // 0x00 = 2g / 16384, 0x08 = 4g / 8192, 0x10 = 8g / 4096, 0x18 = 16g / 2048
//         {MPU9250_ACCEL_CONFIG_2, 0x02},      // enable LowPassFilter  Set Acc LPF 92Hz 7.8ms Delay
//         {MPU9250_USER_CTRL,      0x00},      // Enable AUX
//     };
//
//     imuChassis.id = RM_MPU9250_Read_Reg(MPU9250_WHO_AM_I);  //read id of device,check if MPU9250 or not
//
//     for(index = 0; index < 10; index++) {
//         RM_MPU9250_Write_Reg(MPU9250_Init_Data[index][0], MPU9250_Init_Data[index][1]);
//         HAL_Delay(1);
//     }
//     RM_IMU_MPU9250_GetData();
//     // RM_MPU9250_UPDATE_ACC_ANGLE();
//     // imuChassis.corrected_angle_data.init_acc_x = imuChassis.corrected_angle_data.acc_x;
//     // imuChassis.corrected_angle_data.init_acc_y = imuChassis.corrected_angle_data.acc_y;
//     // imuChassis.corrected_angle_data.init_acc_z = imuChassis.corrected_angle_data.acc_z;
//     // imuChassis.corrected_angle_data.x = imuChassis.corrected_angle_data.init_acc_x;
//     // imuChassis.corrected_angle_data.y = imuChassis.corrected_angle_data.init_acc_y;
//     // imuChassis.corrected_angle_data.z = imuChassis.corrected_angle_data.init_acc_z;
//     return 0;
// }
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Get 6 Axis Data from MPU 6500
// 	Log
// 		12/07/17 Nickel Liang	First Draft
//         12/27/17 Nickel Liang   Move mpu_buff
//         12/28/17 Nickel Liang   Decode
// *******************************************************************************/
// void RM_IMU_MPU9250_GetData(void) {
//     uint8_t mpu_buff[14];
//     // Must read 14 all together
//     RM_MPU9250_Read_Regs(MPU9250_ACCEL_XOUT_H, mpu_buff, 14);
//
//     float acceFactor = 4096.0; // Check datasheet, 8g = 4096
//     imuChassis.acce.rx = (int16_t)(mpu_buff[0]<<8 | mpu_buff[1]);
//     imuChassis.acce.ry = (int16_t)(mpu_buff[2]<<8 | mpu_buff[3]);
//     imuChassis.acce.rz = (int16_t)(mpu_buff[4]<<8 | mpu_buff[5]);
//     imuChassis.acce.x = (float)(imuChassis.acce.rx/acceFactor);
//     imuChassis.acce.y = (float)(imuChassis.acce.ry/acceFactor);
//     imuChassis.acce.z = (float)(imuChassis.acce.rz/acceFactor);
//
//     float roomTempOffset = 0; // 21 C
//     float tempSensitivity = 333.87; // Datasheet p12
//     imuChassis.rtemp = (int16_t)(mpu_buff[6]<<8 | mpu_buff[7]);
//     imuChassis.temp = (float)(((imuChassis.rtemp-roomTempOffset)/tempSensitivity) + 21); // Register map p33
//
//     float gyroFactor = 16.4; // Check datasheet, 2000dps = 16.4
//     imuChassis.gyro.rx = (int16_t)(mpu_buff[8]<<8 | mpu_buff[9]);
//     imuChassis.gyro.ry = (int16_t)(mpu_buff[10]<<8 | mpu_buff[11]);
//     imuChassis.gyro.rz = (int16_t)(mpu_buff[12]<<8 | mpu_buff[13]);
//     imuChassis.gyro.x = (float)(imuChassis.gyro.rx/gyroFactor); // Get a value in degree
//     imuChassis.gyro.y = (float)(imuChassis.gyro.ry/gyroFactor);
//     imuChassis.gyro.z = (float)(imuChassis.gyro.rz/gyroFactor);
//
//     // Actually the following code won't work since we need to get all six data from one single read,
//     // but we cannot achieve that due to read I2C through SPI limitation.
//     // imuChassis.mag.rx = (int16_t)(RM_IST8310_Read_Reg(IST8310_R_XL)<<8 | RM_IST8310_Read_Reg(IST8310_R_XM));
//     // imuChassis.mag.ry = (int16_t)(RM_IST8310_Read_Reg(IST8310_R_YL)<<8 | RM_IST8310_Read_Reg(IST8310_R_YM));
//     // imuChassis.mag.rz = (int16_t)(RM_IST8310_Read_Reg(IST8310_R_ZL)<<8 | RM_IST8310_Read_Reg(IST8310_R_ZM));
// }
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Debug utility function
// 	Log
// 		12/27/17 Nickel Liang	First Draft
// *******************************************************************************/
// void RM_DEBUG_IMU9250_PrintRaw(void) {
//     RM_IMU_MPU9250_GetData();
//     // RM_IMU_MPU9250_GetData();
//     RM_PRINTF("[  RAW  ] ID %02x | ", imuChassis.id);
//     RM_PRINTF("Acce X %d \tY %d \tZ %d \t| ", imuChassis.acce.rx, imuChassis.acce.ry, imuChassis.acce.rz);
//     RM_PRINTF("Gyro X %d \tY %d \tZ %d \t| ", imuChassis.gyro.rx, imuChassis.gyro.ry, imuChassis.gyro.rz);
//     // RM_PRINTF("Magn X %d \tY %d \tZ %d \t| ", imuChassis.mag.rx, imuChassis.mag.ry, imuChassis.mag.rz);
//     RM_PRINTF("Temp %d\r\n", imuChassis.rtemp);
// }
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Debug utility function
// 	Log
// 		12/27/17 Nickel Liang	First Draft
// *******************************************************************************/
// void RM_DEBUG_IMU9250_PrintDecoded(void) {
//     RM_IMU_MPU9250_GetData();
//     RM_PRINTF("[DECODED] ID %02x | ", imuChassis.id);
//     RM_PRINTF("Acce X %f \tY %f \tZ %f \t| ", imuChassis.acce.x, imuChassis.acce.y, imuChassis.acce.z);
//     RM_PRINTF("Gyro X %f \tY %f \tZ %f \t| ", imuChassis.gyro.x, imuChassis.gyro.y, imuChassis.gyro.z);
//     RM_PRINTF("Temp %f\r\n", imuChassis.temp);
// }
//
// void RM_DEBUG_IMU9250_PrintKalman(void){
//     RM_PRINTF("[Filtered] ID %02x | ", imuChassis.id);
//     RM_PRINTF("Kalman_Gyro X %f \tY %f \tZ %f \t| ", imuChassis.corrected_angle_data.x, imuChassis.corrected_angle_data.y, imuChassis.corrected_angle_data.z);
//     RM_PRINTF("Acce X %f \tY %f \tZ %f \t| ", imuChassis.acce.x, imuChassis.acce.y, imuChassis.acce.z);
//     RM_PRINTF("Gyro X %f \tY %f \tZ %f \t| ", imuChassis.gyro.x, imuChassis.gyro.y, imuChassis.gyro.z);
//     RM_PRINTF("Acc_angle X %f \tY %f \tZ %f \t| ", imuChassis.corrected_angle_data.acc_x, imuChassis.corrected_angle_data.acc_y, imuChassis.corrected_angle_data.acc_z);
//     RM_PRINTF("Temp %f\r\n", imuChassis.temp);
// }
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Update accessible acce data
// 	Log
// 		01/12/18 Roger Qiu	First Draft
//         01/14/18 Roger Qiu Bug fix
// *******************************************************************************/
// // static void RM_MPU9250_UPDATE_ACC_ANGLE(void) {
// //     GET_ACC_STATIC_ANGLE(imuChassis.acce.x, imuChassis.acce.y, imuChassis.acce.z, &imuChassis.corrected_angle_data.acc_x, &imuChassis.corrected_angle_data.acc_y, &imuChassis.corrected_angle_data.acc_z);
// //     //imuChassis.corrected_angle_data.acc_z = 0; //TODO: change to mag
// // }
//
// /******************************************************************************
// 	Input
//         - accelerometer Readings
// 	Output
// 	Description
// 		Calculate angles from accelerometer readings
// 	Log
// 		01/12/18 Roger Qiu	First Draft
// *******************************************************************************/
// // static void GET_ACC_STATIC_ANGLE(float grav_x, float grav_y, float grav_z, float *ret_x, float *ret_y, float *ret_z){
// //     *ret_x = atan2(grav_y, grav_z) * rad_2_deg; //roll
// //     *ret_y = atan2(grav_x, grav_z) * rad_2_deg; //pitch
// //     float grav_scalar = sqrt(grav_x * grav_x + grav_y * grav_y + grav_z * grav_z);
// //     /*
// //     *ret_x = acos(grav_x / grav_scalar) * rad_2_deg;
// //     *ret_y = acos(grav_y / grav_scalar) * rad_2_deg;
// //     */
// //     *ret_z = acos(grav_z / grav_scalar) * rad_2_deg;
// // }
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Need to be regularly called to maintain the accessible data
// 	Log
// 		01/12/18 Roger Qiu	First Draft
// *******************************************************************************/
// // void RM_MPU9250_ANGLE_UPDATE(void){
// //     RM_IMU_MPU9250_GetData();
// //     RM_MPU9250_UPDATE_ACC_ANGLE();
// //     RM_MPU9250_KALMAN_FILTER_UPDATE(imuChassis.gyro.x, imuChassis.corrected_angle_data.acc_x - imuChassis.corrected_angle_data.init_acc_x, &imuChassis.corrected_angle_data.x, p_k_x);
// //     RM_MPU9250_KALMAN_FILTER_UPDATE(imuChassis.gyro.y, imuChassis.corrected_angle_data.acc_y - imuChassis.corrected_angle_data.init_acc_y, &imuChassis.corrected_angle_data.y, p_k_y);
// //     RM_MPU9250_DISCRETE_INTEGRAL(imuChassis.gyro.z, &imuChassis.corrected_angle_data.z);
// //     //RM_MPU9250_KALMAN_FILTER_UPDATE(imuChassis.gyro.z, imuChassis.corrected_angle_data.acc_z, &imuChassis.corrected_angle_data.z, p_k_z);
// // }
// //
// // static void RM_MPU9250_DISCRETE_INTEGRAL(const float gyro_k, float *angle_holder){
// //     float rate = gyro_k - z_bias;
// //     *angle_holder = *angle_holder + dt * rate;
// // }
//
// /******************************************************************************
// 	Input
//         - gyro_k: angular velocity from gyro.
//         - acc_angle: angle measured by accelerometer
//         - *a_angle: pointer to a axis
//         - *p_k: pointer to covariance matrix of an axis
// 	Description
// 		Calculate Kalman filter adjusted angle
// 	Log
// 		12/12/17 Roger Qiu	First Draft
//         01/12/18 Roger Qiu Bug Fix; parameter updated; it's now working!
// *******************************************************************************/
// // static void RM_MPU9250_KALMAN_FILTER_UPDATE(const float gyro_k, const float acc_angle, float *a_angle, double p_k[2][2]){ //imuChassis.gyro.x, acc_angle, desired_ang
// //     float rate = gyro_k - q_bias;
// //     float p_cache[4] = {0, 0, 0, 0};
// //     //double p_k[2][2] = *p_to_mat;
// //     //EQ 1 - A priori state estimate
// //     //ideal estimated angle
// //     *a_angle += rate * dt; //a priori estimate
// //     //EQ 2 - A priori estimate covariance
// //     //p_k_k-1 (estimate from past observations)
// //     p_cache[0] = q_angle_x - p_k[0][1] - p_k[1][0] + dt * p_k[1][1];
// //     p_cache[1] = -1 * p_k[1][1];
// //     p_cache[2] = -1 * p_k[1][1];
// //     p_cache[3] = q_gyro_x;
// //     p_k[0][0] += p_cache[0] * dt; // dt^2 << 0. Ignore.
// //     p_k[0][1] += p_cache[1] * dt;
// //     p_k[1][0] += p_cache[2] * dt;
// //     p_k[1][1] += p_cache[3] * dt;
// //     //EQ 3 - Optimal Kalman Gain
// //     float angle_err = acc_angle - *a_angle;
// //     float E = r_angle + p_k[0][0];
// //     float k_0 = p_k[0][0] / E;
// //     float k_1 = p_k[1][0] / E;
// //     //EQ 4 (and 5) - A posteriori estimate covariance (update original covariance matrix)
// //     /*
// //     p_k[1][0] = k_1 * p_k[0][0];
// //     p_k[1][1] = k_1 * p_k[0][1];
// //     p_k[0][0] = k_0 * p_k[0][0];
// //     p_k[0][1] = k_0 * p_k[0][1];
// //     */
// //     p_k[0][0] -= k_0 * p_k[0][0];
// //     p_k[0][1] -= k_0 * p_k[0][1];
// //     p_k[1][0] -= k_1 * p_k[0][0];
// //     p_k[1][1] -= k_1 * p_k[0][1];
// //     //calculate adjusted angle
// //     *a_angle += k_0 * angle_err;
// //     q_bias += k_1 * angle_err;
// //     //return *a_angle;
// // }
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Pull NSS Pin Low. Select MPU9250.
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// static void RM_MPU9250_NSS_Low(void) {
//     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
// }
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Pull NSS Pin High. Invert Select MPU9250.
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// static void RM_MPU9250_NSS_High(void) {
//     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
// }
//
// /******************************************************************************
// 	Input
//         reg register to be written
//         data data to be written
// 	Output
//         0
// 	Description
// 		Write a register on MPU9250
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// static uint8_t RM_MPU9250_Write_Reg(uint8_t const reg, uint8_t const data) {
//     static uint8_t MPU_Rx, MPU_Tx;
//     RM_MPU9250_NSS_Low();
//     MPU_Tx = reg & 0x7f;
//     HAL_SPI_TransmitReceive(&hspi4, &MPU_Tx, &MPU_Rx, 1, 55);
//     MPU_Tx = data;
//     HAL_SPI_TransmitReceive(&hspi4, &MPU_Tx, &MPU_Rx, 1, 55);
//     RM_MPU9250_NSS_High();
//     return 0;
// }
//
// /******************************************************************************
// 	Input
//         reg register to be read
// 	Output
//         Received data
// 	Description
// 		Read a register on MPU9250
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// static uint8_t RM_MPU9250_Read_Reg(uint8_t const reg) {
//     static uint8_t MPU_Rx, MPU_Tx;
//     RM_MPU9250_NSS_Low();
//     MPU_Tx = reg | 0x80;
//     HAL_SPI_TransmitReceive(&hspi4, &MPU_Tx, &MPU_Rx, 1, 55);
//     HAL_SPI_TransmitReceive(&hspi4, &MPU_Tx, &MPU_Rx, 1, 55);
//     RM_MPU9250_NSS_High();
//     return MPU_Rx;
// }
//
// /******************************************************************************
// 	Input
//         regAddr start addr
//         pData received data
//         len how many consecutive registers
// 	Output
//         0
// 	Description
// 		Read registers on MPU9250
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// static uint8_t RM_MPU9250_Read_Regs(uint8_t const regAddr, uint8_t *pData, uint8_t len) {
//     static uint8_t MPU_Rx, MPU_Tx, MPU_Tx_buff[14] = {0xff};
//     RM_MPU9250_NSS_Low();
//     MPU_Tx = regAddr | 0x80;
//     MPU_Tx_buff[0] = MPU_Tx;
//     HAL_SPI_TransmitReceive(&hspi4, &MPU_Tx, &MPU_Rx, 1, 55);
//     HAL_SPI_TransmitReceive(&hspi4, MPU_Tx_buff, pData, len, 55);
//     RM_MPU9250_NSS_High();
//     return 0;
// }
//
// /******************************************************************************
// 	Input
//         percent PWM parameter
// 	Output
//         0
// 	Description
// 		Read registers on MPU9250
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// // static void RM_MPU9250_Heater(uint8_t percent) {
// //
// // }
//
// /******************************************************************************
// 	Input
// 	Output
//         0 - Success
//         Other - Error
// 	Description
// 		Initialize Onboard IST8310 Magnetic Sensor
// 	Log
// 		12/07/17 Nickel Liang	First Draft
//         12/30/17 Nickel Liang   Comments and fix
//     NOTE
//         Do not use IST8310, hardware bug.
// *******************************************************************************/
// /*uint8_t RM_IMU_IST8310_Init(void) {
//     RM_MPU9250_Write_Reg(MPU9250_USER_CTRL, 0x30); // Enable the I2C Master I/F module, Reset I2C Slave module
//     HAL_Delay(10);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_MST_CTRL, 0x0d); // 400kHz
//     HAL_Delay(10);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_ADDR, IST8310_ADDRESS); // Write from slave 1
//     HAL_Delay(10);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV4_ADDR, 0x80 | IST8310_ADDRESS); // Read from slave 4
//     HAL_Delay(10);
//
//     RM_IST8310_Write_Reg(IST8310_R_CONFB, 0x01);
//     if(IST8310_DEVICE_ID_A != RM_IST8310_Read_Reg(IST8310_WHO_AM_I))
//         return 1; //error
//     HAL_Delay(10);
//
//     RM_IST8310_Write_Reg(IST8310_R_CONFA, 0x00);
//     if(RM_IST8310_Read_Reg(IST8310_R_CONFA) != 0x00)
//         return 2;
//     HAL_Delay(10);
//
//     RM_IST8310_Write_Reg(IST8310_R_CONFB, 0x00);
//     if(RM_IST8310_Read_Reg(IST8310_R_CONFB) != 0x00)
//         return 3;
//     HAL_Delay(10);
//
//     RM_IST8310_Write_Reg(IST8310_AVGCNTL, 0x24);
//     if(RM_IST8310_Read_Reg(IST8310_AVGCNTL) != 0x24)
//         return 4;
//     HAL_Delay(10);
//
//     RM_IST8310_Write_Reg(IST8310_PDCNTL, 0xc0);
//     if(RM_IST8310_Read_Reg(IST8310_PDCNTL) != 0xc0)
//         return 5;
//     HAL_Delay(10);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_CTRL, 0x00);
//     HAL_Delay(10);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV4_CTRL, 0x00);
//     HAL_Delay(10);
//
//     RM_MPU_Read_IST_Init(IST8310_ADDRESS, IST8310_R_XL, 0x06);
//     HAL_Delay(100);
//     return 0;
// }
// */
//
// /******************************************************************************
// 	Input
//         addr register to be written
//         data data to be written
// 	Output
//         0
// 	Description
// 		Write a IST8310 register through MPU9250
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// /*static uint8_t RM_IST8310_Write_Reg(uint8_t addr, uint8_t data) {
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_CTRL, 0x00);
//     HAL_Delay(2);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_REG, addr);
//     HAL_Delay(2);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_DO, data);
//     HAL_Delay(2);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_CTRL, 0x080 | 0x01);
//     HAL_Delay(10);
//     return 0;
// }
// */
//
// /******************************************************************************
// 	Input
//         addr register to be written
// 	Output
//         received data
// 	Description
// 		Read a IST8310 register through MPU9250
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// /*static uint8_t RM_IST8310_Read_Reg(uint8_t addr) {
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV4_REG, addr);
//     HAL_Delay(10);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV4_CTRL, 0x80);
//     HAL_Delay(10);
//     uint8_t data = RM_MPU9250_Read_Reg(MPU9250_I2C_SLV4_DI);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV4_CTRL, 0x00);
//     HAL_Delay(10);
//     return data;
// }
// */
//
// /******************************************************************************
// 	Input
//         device_address
//         reg_base_addr
//         data_num number of data
// 	Output
// 	Description
// 		Configure MPU9250 to read data from IST8310
//         Initialize the MPU9250 I2C Slave0 for I2C reading
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// /*static void RM_MPU_Read_IST_Init(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num) {
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_ADDR, device_address);
//     HAL_Delay(2);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_REG, IST8310_R_CONFA);
//     HAL_Delay(2);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_DO, IST8310_ODR_MODE);
//     HAL_Delay(2);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV0_ADDR, 0x80 | device_address);
//     HAL_Delay(2);
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV0_REG, reg_base_addr);
//     HAL_Delay(2);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV4_CTRL, 0x03);
//     HAL_Delay(2);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
//     HAL_Delay(2);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV1_CTRL, 0x80 | 0x01);
//     HAL_Delay(6);
//
//     RM_MPU9250_Write_Reg(MPU9250_I2C_SLV0_CTRL, 0x80 | data_num);
//     HAL_Delay(7);
// }
// */
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Set MPU9250 Acceleration data resolution
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// /*static uint8_t RM_MPU9250_Set_Accel_Fsr(uint8_t fsr) {
//     return RM_MPU9250_Write_Reg(MPU9250_ACCEL_CONFIG, fsr<<3);
// }
// */
//
// /******************************************************************************
// 	Input
// 	Output
// 	Description
// 		Set MPU9250 Gyro data resolution
// 	Log
// 		12/07/17 Nickel Liang	First Draft
// *******************************************************************************/
// /*static uint8_t RM_MPU9250_Set_Gyro_Fsr(uint8_t fsr) {
//     return RM_MPU9250_Write_Reg(MPU9250_GYRO_CONFIG, fsr<<3);
// }
// */
