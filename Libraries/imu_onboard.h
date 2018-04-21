#ifndef _IMU_ONBOARD_H_
#define _IMU_ONBOARD_H_

#include "bsp_imu.h"
#include <math.h>


#define DT          0.02    // measurement time interval. RTOS should give exact time to me.
#define QANGLE      0.00002 // angle noise variance
#define QGYRO       0.0065  // gyro noise variance
#define RANGLE      0.1     // measurement noise covariance cov(gyro, angle)
#define STATIC_LIM  4       // if (all) gyro values are smaller than this lim, we assume it's static
#define STATIC_TURN 20      // we only start to update zero bias if the robots are static in 10 measurements
#define IMUSAMPLES  200     // how many gyro samples to grab

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
 * get current yaw angle for chassis
 * @brief
 * @return  current yaw angle for chassis in degree (without wrap around. E.g., you may get 370)
 */
float get_chassis_yaw_angle(void);

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
