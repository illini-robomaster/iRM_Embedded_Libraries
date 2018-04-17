/**
 * @author  Nickel_Liang
 * @date    2018-04-12
 * @file    rm_rtos.h
 * @brief   RTOS injection for the project
 * @log     2018-04-13 Nickel_Liang
 * @note    Functions and declarations for rtos should be written in this file.
 * @note    This file is included in ../Src/freertos.c to minimize the impact of future driver and middleware update.
 * @note    All functions should begin with "RM_" due to the nature of interaction with driver codes.
 */

#ifndef _RM_RTOS_H_
#define _RM_RTOS_H_

/* Includes */
#include "rm_config.h"
#include "bsp_print.h"
#include "bsp_buzzer.h"
#include "bsp_led.h"
#include "bsp_laser.h"

/* RTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Test includes */
#include "motor.h"

#define TEST_CAN        1
#define MOTOR_6623      0x209
#define MOTOR_6623_TX   0x2FF

#define MOTOR_3510      0x205
#define MOTOR_3510_TX   0x1FF

#define MOTOR_3508      0x201
#define MOTOR_3508_TX   0x200

#define MOTOR_2006      0x202
#define MOTOR_2006_TX   0x200

motor_t test_6623;
motor_t test_3508;
motor_t test_3510;
motor_t test_2006;

/**
 * Initialize after RTOS is initialized.
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_Init(void) {
    /* Initialize RTOS */
    return;
}

/**
 * Initialize RTOS MUTEX
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_MUTEX_Init(void) {
    return;
}

/**
 * Initialize RTOS SEMAPHORES
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_SEMAPHORES_Init(void) {
    return;
}

/**
 * Initialize RTOS TIMERS
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_TIMERS_Init(void) {
    return;
}

/**
 * Initialize RTOS THREADS
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_THREADS_Init(void) {
    return;
}

/**
 * Initialize RTOS QUEUES
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_QUEUES_Init(void) {
    return;
}

/**
 * DefaultTask Jobs
 *
 * @param  argument   Same parameter passed in in Src/freertos.c
 * @author Nickel_Liang
 * @date   2018-04-13
 */
extern inline void RM_RTOS_Default_Task(void const * argument) {
    /* Add codes to initialize default thread here */
    print("Enter default task.\n");
    // test_3510.extra_gimbal
    motor_id_init(&test_3510, MOTOR_3510, TEST_CAN);
    motor_id_init(&test_3508, MOTOR_3508, TEST_CAN);
    motor_id_init(&test_2006, MOTOR_2006, TEST_CAN);
    motor_id_init(&test_6623, MOTOR_6623, TEST_CAN);
    /* There must be a while loop here. */
    while(1) {
        get_extra_gimbal_data(&test_3510);
        get_chassis_data(&test_3508);
        get_poke_data(&test_2006);
        get_gimbal_data(&test_6623);
        osDelay(1);
    }
}

/**
 * RTOS is ready.
 *
 * @author Nickel_Liang
 * @date   2018-04-16
 */
extern inline void RM_RTOS_Ready() {
    /* Indicate RTOS booted. Ready to battle. */
    buzzer_sing_song(startup, 1);
    LED_green_toggle();
    laser_on();
    print("RTOS initialzed. Ready to battle.\n");
}

#endif
