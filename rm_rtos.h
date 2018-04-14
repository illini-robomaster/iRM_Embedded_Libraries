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

#ifndef _RM_RTOS_
#define _RM_RTOS_

/* Includes */
#include "rm_config.h"

/**
 * Initialize after RTOS is initialized.
 *
 * @author  Nickel_Liang
 * @date    2018-04-13
 */
extern inline void RM_RTOS_Init(void) {
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

    /* There must be a while loop here. Use empty for loop to prevent compiler from complaining. */
    for(;;) {

    }
}

#endif
