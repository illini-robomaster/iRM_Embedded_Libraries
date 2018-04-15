/**
 * @author  Nickel_Liang
 * @date    2018-04-12
 * @file    rm_main.h
 * @brief   RM MAIN Insertion file.
 * @log     2018-04-13 Nickel_Liang
 * @note    Functions and declarations for main should be written in this file.
 * @note    This file is included in ../Src/main.c to minimize the impact of future driver and middleware update.
 * @note    All functions should begin with "RM_" due to the nature of interaction with driver codes.
 */

#ifndef _RM_MAIN_
#define _RM_MAIN_

/* Includes */
#include "rm_config.h"
#include "rm_printf.h"
/**
 * Initialize peripherals before get into RTOS
 *
 * @author  Nickel Liang
 * @date    2018-04-13
 */
extern inline void RM_Main_Init(void) {
    while (1){
        RM_PRINTF("objk\n\n\n");
    }
    return;
}

#endif
