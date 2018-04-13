/**
 * @author  Nickel_Liang
 * @date    2018-04-12
 * @file    rm_config.h
 * @brief   RM General Constant Configuration
 * @log     2018-04-13 Nickel_Liang
 * @note    Macros should be injected via cmake
 * @note    All robot dependent constants and parameters should be define in this file.
 * @note    This file should be included in all other .h files.
 */

#ifndef _RM_CONFIG_
#define _RM_CONFIG_

/* Inject macros here if you do not know how to... */

/* Constants and Parameters for Debug Configurations */
/* @note    Macros should be injected via cmake */
#ifdef  DEBUG

#endif

/* Constants and Parameters for All Infantry Robots */
/* @note    Macros should be injected via cmake */
#ifdef  INFANTRY
// Pin define..
#endif

/* Constants and Parameters for Infantry Robot 1 */
/* @note    Macros should be injected via cmake */
#ifdef  INFANTRY_1

#endif

/* Constants and Parameters for Infantry Robot 2 */
/* @note    Macros should be injected via cmake */
#ifdef  INFANTRY_2
#endif

/* Constants and Parameters for Infantry Robot 3 */
/* @note    Macros should be injected via cmake */
#ifdef  INFANTRY_3
#endif

/* Constants and Parameters for Hero Robot */
/* @note    Macros should be injected via cmake */
#ifdef  HERO
#endif

/* Constants and Parameters for Engineering Robot */
/* @note    Macros should be injected via cmake */
#ifdef  ENGINEERING
#endif

/* Constants and Parameters for Supply Station */
/* @note    Macros should be injected via cmake */
#ifdef  SUPPLY
#endif

/* Constants and Parameters for Sentry Robot */
/* @note    Macros should be injected via cmake */
#ifdef  SENTRY
#endif

/* General Parameters */

/**
 * @todo    Add support for UART port selection
 * @todo    Add support for more flexible CAN setting
 */

/* General Constants */
#define PI          3.14159265358979f

#endif
