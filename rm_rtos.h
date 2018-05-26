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
// #include "rm_test.h"
#include "rm_config.h"
#include "bsp_print.h"
#include "bsp_buzzer.h"
#include "bsp_led.h"
#include "bsp_laser.h"
#include "bsp_sdio.h"
#include "task_referee.h"
#include "task_imu.h"
#include "task_motion.h"

#include "test_sdio.h"

/* RTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

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
    // referee_task_create();
    // imu_task_create();
    motion_task_create();
}

/**
 * Initialize RTOS QUEUES
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_QUEUES_Init(void) {
    sdio_queue_init();
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
    //osTimerStart(gimbal_timer_id,gimbal_period);
    //osTimerStart(chassis_timer_id, chassis_period);
    // run_all_tests();
    // test_sdio();
    /* There must be a while loop here. */
    while(1) {
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
    led_green_on();
    laser_on();
    print("RTOS initialzed. Ready to battle.\n");
}

#endif
