#ifndef _RM_TEST_H_
#define _RM_TEST_H_

#include "test_motor.h"
#include "test_pid.h"
#include "test_dbus.h"
#include "test_bsp_can.h"
#include "test_servo.h"
#include "test_bsp_imu.h"
#include "test_imu.h"

/* Test utility */
#define PASS    1
#define FAIL    0
#define ON      1
#define OFF     0
#define TEST_HEADER                 \
    print("[TEST] Running %s at %s:%d\n", __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)   \
    print("[TEST] %s %s\n", name, (result) ? "PASS" : "FAIL");

/* Test config */
#define TEST_PID        OFF
#define TEST_MOTOR      OFF
#define TEST_DBUS       OFF
#define TEST_BSP_CAN    OFF
#define TEST_SERVO      OFF
#define TEST_BSP_IMU    OFF
#define TEST_IMU        ON

/* TODO: test case not finished yet */
extern inline void run_all_tests() {
    if (TEST_PID == ON)
        test_pid();
    if (TEST_MOTOR == ON)
        test_motor();
    if (TEST_DBUS == ON)
        TEST_OUTPUT("DBUS TEST", test_dbus());
    if (TEST_BSP_CAN == ON)
        TEST_OUTPUT("BSP CAN TEST", test_bsp_can());
    if (TEST_SERVO == ON)
        test_servo();
    if (TEST_BSP_IMU == ON)
        TEST_OUTPUT("BSP IMU TEST", test_bsp_imu());
    if (TEST_IMU == ON)
        test_imu();
}

#endif
