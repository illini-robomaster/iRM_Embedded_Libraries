/******************************************************************************
	Description
		typedef for easier use
        Include Generic C Libs
    Log
		11/23/17 Nickel Liang	First Draft
		12/05/17 Nickel Liang   Minor improvements, Add libs
        01/02/18 Nickel Liang   Add libs
*******************************************************************************/

#ifndef __MYTYPE
#define __MYTYPE

/* HAL Lib */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/* Standard C Lib */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

/* MX Generate Lib */
// #include "bsp_driver_sd.h"
#include "can.h"
#include "dma.h"
#include "fatfs.h"
#include "ffconf.h"
#include "gpio.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* Commonly Used RM Lib */
#include "rm_printf.h"
#include "bsp_can.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "buzzer.h"
#include "dbus.h"
#include "gimbal.h"
#include "imu_onboard.h"
#include "chassis.h"
#include "judge_system.h"
#include "key.h"
#include "laser.h"
#include "led.h"
#include "launcher.h"
#include "pid.h"

/* Commonly Used Variables*/
#define pi 3.1415926
#define rad_2_deg 57.29578

/* Commonly Used Functions */
#define ABS(x) ((x>0) ? (x) : (-x))

/*static double RMSqrt(double x){
    double i=0;
    if (x<=0) return 0;
    for (i=1; i*i<=x; i++) {;}
    return (i-0.5);
}*/

/* Communicate signal*/
#define LAUNCHER_TASK_EXE_SIGNAL (int32_t)(1<<0)


/* Commonly Used Types */
typedef uint8_t 	u8;
typedef uint16_t 	u16;
typedef uint32_t 	u32;

typedef int8_t 		s8;
typedef int16_t 	s16;
typedef int32_t		s32;

typedef volatile uint8_t 	vu8;
typedef volatile uint16_t 	vu16;
typedef volatile uint32_t 	vu32;

typedef volatile int8_t 	vs8;
typedef volatile int16_t 	vs16;
typedef volatile int32_t	vs32;

#endif
