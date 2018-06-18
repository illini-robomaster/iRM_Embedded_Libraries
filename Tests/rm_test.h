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

#ifndef _RM_TEST_H_
#define _RM_TEST_H_

#include "test_motor.h"
#include "test_pid.h"
#include "test_dbus.h"
#include "test_bsp_can.h"
#include "test_servo.h"
#include "test_bsp_imu.h"
#include "test_imu.h"
#include "test_mouse.h"
#include "test_chassis.h"
#include "test_haptor.h"
#include "test_tx2.h"
#include "test_sdio.h"
#include "test_key.h"
#include "test_referee.h"
#include "test_bsp_adc.h"
#include "test_bsp_pwm.h"

/* Test utility */
#define PASS    1
#define FAIL    0

#define RM_RTOS 0
#define RM_MAIN 1

#define TEST_HEADER                 \
    print("[TEST] Running %s at %s:%d\n", __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)   \
    print("[TEST] %s : %s\n", name, (result) ? "PASS" : "FAIL");

/* Test options */
#define RUN_IN          RM_MAIN // RM_RTOS : Run test in RTOS default task @todo Not yet implemented
                                // RM_MAIN : Run test in main function

/* Test config */
#define TEST_PID        OFF
#define TEST_MOTOR      OFF
#define TEST_DBUS       OFF
#define TEST_BSP_CAN    OFF
#define TEST_SERVO      OFF
#define TEST_BSP_PWM    OFF
#define TEST_BSP_IMU    OFF
#define TEST_IMU        OFF
#define TEST_MOUSE      OFF
#define TEST_HAPTOR     OFF
#define TEST_CHASSIS    OFF
#define TEST_TX2        OFF
#define TEST_REFEREE    OFF
#define TEST_BSP_ADC    OFF
#define TEST_SDIO       DISABLE
#define TEST_KEY        OFF

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
    if (TEST_BSP_PWM == ON)
        test_bsp_pwm();
    if (TEST_BSP_IMU == ON)
        TEST_OUTPUT("BSP IMU TEST", test_bsp_imu());
    if (TEST_IMU == ON)
        test_imu();
    if (TEST_MOUSE == ON)
        test_mouse();
    if (TEST_CHASSIS == ON)
        test_chassis();
    if (TEST_HAPTOR == ON)
        test_haptor();
    if (TEST_TX2 == ON)
        test_tx2();
    if (TEST_REFEREE == ON)
        test_referee();
    if (TEST_BSP_ADC == ON)
        test_bsp_adc();
    if (TEST_SDIO == ON)
        TEST_OUTPUT("SDIO TEST", test_sdio());
    if (TEST_KEY == ON)
        TEST_OUTPUT("KEY TEST", test_key());
}

#endif
