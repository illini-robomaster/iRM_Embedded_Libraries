#include "imu_onboard.h"

imu_onboard_t imuBoard;

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

void print_imu_data(void){
    print("Angle X %.2f \tY %.2f", imuBoard.angle[0], imuBoard.angle[1]);
    print(" \tZ %.2f \t| ", imuBoard.angle[2]);
    print("\r\n");
    print("ZBias X %.2f \tY %.2f", imuBoard.angle_zero_bias[0], imuBoard.angle_zero_bias[1]);
    print(" \tZ %.2f \t| ", imuBoard.angle_zero_bias[2]);
    print("\r\n");
}

float get_chassis_yaw_angle(void) {
    return 0;
}

void onboard_imu_lib_init(void){
    print("Initing and calibrating onboard imu\r\n");
    for(int i = 0; i < 3; ++i){
        imuBoard.angle[i] = 0;
        for(int j = 0; j < 2; ++j){
            for(int k = 0; k < 2; ++k){
                imuBoard.p_k[i][j][k] = 0;
            }
        }
    }
    // grab some samples of gyro data for an initial estimate of zero biases
    imuBoard.total_measurement_count = 0;
    while(imuBoard.total_measurement_count < IMUSAMPLES) {
        mpu6500_get_data(&(imuBoard.my_raw_imu));
        //TODO: add foolproof logic or assume none of us is a fool.
        update_zero_bias();
    }
    update_acc_angle();
    for(int AXIS = 0; AXIS < 3; ++AXIS) {
        imuBoard.init_acc_angle[AXIS] = imuBoard.acc_angle[AXIS];
        imuBoard.angle[AXIS] = 0;
    }
    imuBoard.static_measurement_count = 0;
}

void onboard_imu_update(void){
    mpu6500_get_data(&(imuBoard.my_raw_imu));
    update_acc_angle();
    if(MY_ABS(imuBoard.my_raw_imu.gyro.x) < STATIC_LIM
                && MY_ABS(imuBoard.my_raw_imu.gyro.y) < STATIC_LIM && MY_ABS(imuBoard.my_raw_imu.gyro.z) < STATIC_LIM){
        ++imuBoard.static_measurement_count;
    } else {
        imuBoard.static_measurement_count = 0;
    }
    if(imuBoard.static_measurement_count > STATIC_TURN){
        // Robot has been static for a while. We calibrate IMU accordingly.
        update_zero_bias();
    } else {
        // if the robot is static, we don't need to update angle
        kalman_filter_update(IMU_X);
        kalman_filter_update(IMU_Y);
        discrete_integral(IMU_Z);
    }
}

void update_zero_bias(void){
    float* pgyro = (float*)(&imuBoard.my_raw_imu.gyro.x);
    for(int axis = 0; axis < 3; ++axis){
        imuBoard.angle_zero_bias[axis]
                = (imuBoard.angle_zero_bias[axis] * imuBoard.total_measurement_count
                                + *(pgyro + axis)) / (++imuBoard.total_measurement_count);
    }
}

void discrete_integral(imu_axis_t desired_axis){
    float* pgyro = (float*)(&imuBoard.my_raw_imu.gyro.x);
    float rate = *(pgyro + desired_axis) - imuBoard.angle_zero_bias[desired_axis];
    imuBoard.angle[desired_axis] += rate * DT;
}

float kalman_filter_update(imu_axis_t desired_axis){
    float* pgyro = (float*)(&imuBoard.my_raw_imu.gyro.x);
    float rate = *(pgyro + desired_axis) - imuBoard.angle_zero_bias[desired_axis];
    float p_cache[4] = {0, 0, 0, 0};
    //EQ 1 - A priori state estimate
    //ideal estimated angle
    float temp_angle = imuBoard.angle[desired_axis] + rate * DT;
    //EQ 2 - A priori estimate covariance
    //p_{k}_{k-1} (estimate from past observations)
    p_cache[0] = QANGLE - imuBoard.p_k[desired_axis][0][1] -
                        imuBoard.p_k[desired_axis][1][0] + DT * imuBoard.p_k[desired_axis][1][1];
    p_cache[1] = -1 * imuBoard.p_k[desired_axis][1][1];
    p_cache[2] = -1 * imuBoard.p_k[desired_axis][1][1];
    p_cache[3] = QGYRO;
    imuBoard.p_k[desired_axis][0][0] += p_cache[0] * DT; // dt^2 << 0. Ignore.
    imuBoard.p_k[desired_axis][0][1] += p_cache[1] * DT;
    imuBoard.p_k[desired_axis][1][0] += p_cache[2] * DT;
    imuBoard.p_k[desired_axis][1][1] += p_cache[3] * DT;
    //EQ 3 - Optimal Kalman Gain
    float angle_err = (imuBoard.acc_angle[desired_axis] - imuBoard.init_acc_angle[desired_axis]) - temp_angle;
    float E = RANGLE + imuBoard.p_k[desired_axis][0][0];
    float k_0 = imuBoard.p_k[desired_axis][0][0] / E;
    float k_1 = imuBoard.p_k[desired_axis][1][0] / E;
    //EQ 4 (and 5) - A posteriori estimate covariance (update original covariance matrix)
    imuBoard.p_k[desired_axis][0][0] -= k_0 * imuBoard.p_k[desired_axis][0][0];
    imuBoard.p_k[desired_axis][0][1] -= k_0 * imuBoard.p_k[desired_axis][0][1];
    imuBoard.p_k[desired_axis][1][0] -= k_1 * imuBoard.p_k[desired_axis][0][0];
    imuBoard.p_k[desired_axis][1][1] -= k_1 * imuBoard.p_k[desired_axis][0][1];
    //calculate adjusted angle
    temp_angle += k_0 * angle_err;
    imuBoard.angle[desired_axis] = temp_angle;
    imuBoard.angle_zero_bias[desired_axis] += k_1 * angle_err;
}

void update_acc_angle(void){
    // assume update angle has already been called
    imuBoard.acc_angle[IMU_X] = atan2(imuBoard.my_raw_imu.acce.y, imuBoard.my_raw_imu.acce.z) * RAD_2_DEG;
    imuBoard.acc_angle[IMU_Y] = atan2(imuBoard.my_raw_imu.acce.x, imuBoard.my_raw_imu.acce.z) * RAD_2_DEG;

    // REWRITE this function if mechanical design change and RM board is relocated
    if(0) {
        float grav_scalar = sqrt(imuBoard.acc_angle[IMU_X] * imuBoard.acc_angle[IMU_X]
                + imuBoard.acc_angle[IMU_Y] * imuBoard.acc_angle[IMU_Y]
                + imuBoard.acc_angle[IMU_Z] * imuBoard.acc_angle[IMU_Z]);
        imuBoard.acc_angle[IMU_Z] = acos(imuBoard.my_raw_imu.acce.z / grav_scalar) * RAD_2_DEG;
    } else {
        imuBoard.acc_angle[IMU_Z] = 0;
    }
}
