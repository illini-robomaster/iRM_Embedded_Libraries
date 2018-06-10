/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#ifndef _IMU_ONBOARD_H_
#define _IMU_ONBOARD_H_

#include "lib_config.h"
#include "bsp_imu.h"
#include <math.h>

#define QANGLE      0.00002f // angle noise variance
#define QGYRO       0.0065f  // gyro noise variance
#define RANGLE      0.1f     // measurement noise covariance cov(gyro, angle)
#define STATIC_LIM  4       // if (all) gyro values are smaller than this lim, we assume it's static
#define STATIC_TURN 20      // we only start to update zero bias if the robots are static in 10 measurements
#define IMUSAMPLES  1000     // how many gyro samples to grab

typedef enum{
    ROLL  = 0,
    PITCH = 1,
    YAW  = 2
} imu_euler_t;

typedef enum{
    IMU_X = 0,
    IMU_Y = 1,
    IMU_Z = 2
} imu_axis_t;

/* angles are all in degrees */
/* Counterclockwise direction: positive */
typedef struct {
    float angle[3];                 // real computed values. Should be good for direct access
    float angle_zero_bias[3];       // regularly updated zero bias data
    float acc_angle[3];             // angles calculated based on accelerometer
    float init_acc_angle[3];        // initial angles calculated based on accelerometer. Used for relative angles
    float p_k[3][2][2];             // Error Covariance matrix
    int   static_measurement_count; // Static measurement count
    int   total_measurement_count;  // Total samples used.
    imu_t my_raw_imu;              // raw imu struct
} imu_onboard_t;

extern imu_onboard_t imuBoard;

/**
 * debug helper function that writes out angles calculated
 * @brief
 */
void print_imu_data(void);

/**
 * Print accelerometer, gyroscope, temp data
 *
 * @param  imu        A valid imu object
 * @author Nickel_Liang
 * @date   2018-04-21
 */
void print_mpu_data(imu_t* imu);

/**
 * Initialize onboard imu. Call this before (regularly) updating values or weird things happen!
 * @brief
 */
void onboard_imu_lib_init(void);

/**
 * Update my imu struct at this very moment. Should be call RIGHT before updating any angle
 * @brief
 */
void onboard_imu_update(void);

/**
 * Update zero bias with current gyroscope data and sampled size in struct. DOES NOT perform sanity check!
 * @brief
 */
void update_zero_bias(void);

/**
 * Update angle of all axes using solely accelerometer data
 * @brief
 */
void update_acc_angle(void);

/**
 * Use discrete integral to update corresponding imu axis based on data in struct.
 * @brief
 * @param  desired_axis which axis you want to update?
 */
void discrete_integral(imu_axis_t desired_axis);

/**
 * Update angle of corresponding axis with kalman filtering based on data in struct.
 * @brief
 * @param  desired_axis which axis to update
 * @return              kalman-filtered calculated value
 */
float kalman_filter_update(imu_axis_t desired_axis);

#endif
