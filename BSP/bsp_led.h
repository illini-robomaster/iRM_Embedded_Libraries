/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-14
 * @file    bsp_led.h
 * @brief   LED board support package
 * @log     2018-04-14 nickelliang
 */

#ifndef _BSP_LED_
#define _BSP_LED_

#include "rm_config.h"
#include "stm32f4xx_hal.h"

/**
 * Turn on red LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void LED_red_on(void);

/**
 * Turn off red LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void LED_red_off(void);

/**
 * Toggle red LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 * @todo   Not tested yet
 */
void LED_red_toggle(void);

/**
 * Turn on green LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void LED_green_on(void);

/**
 * Turn off green LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void LED_green_off(void);

/**
 * Toggle green LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 * @todo   Not tested yet
 */
void LED_green_toggle(void);

/**
 * Turn on all LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void LED_on(void);

/**
 * Turn off all LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 */
void LED_off(void);

/**
 * Toggle all LED
 *
 * @author Nickel_Liang
 * @date   2018-04-14
 * @todo   Not tested yet
 */
void LED_toggle(void);

#endif
