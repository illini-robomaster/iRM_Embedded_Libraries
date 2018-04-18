#ifndef _RM_TEST_H_
#define _RM_TEST_H_

#include "test_motor.h"
#include "test_pid.h"
#include "test_dbus.h"
#include "test_bsp_can.h"

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
#define TEST_DBUS       ON
#define TEST_BSP_CAN    OFF

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
}

#endif
