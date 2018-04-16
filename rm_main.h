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

/** @defgroup library User Library */
/** @defgroup bsp BSP Library */

#ifndef _RM_MAIN_H_
#define _RM_MAIN_H_

/* Includes */
#include "rm_config.h"
#include "bsp_print.h"
#include "bsp_buzzer.h"
#include "bsp_can.h"
#include "bsp_gpio.h"
#include "bsp_led.h"

/**
 * Initialize peripherals before get into RTOS
 *
 * @author  Nickel Liang
 * @date    2018-04-13
 */
extern inline void RM_Main_Init(void) {
    buzzer_init();
    buzzer_sing_song(initialize, 1); // Control board power up.
    LED_red_toggle();
    CAN1_init();
    CAN2_init();
    GPIO_interrupt_init();

    /* Indicate successfully initialized */
    LED_green_toggle();
    print("All peripherals initialized.\n");

    /* Wait for ESC to initialize */
    /* @todo change to more delicate CAN bus condition check function */
    HAL_Delay(STARTUP_DELAY);
    return;
}

#endif
