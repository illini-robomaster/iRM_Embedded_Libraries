/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-15
 * @file    bsp_ist8310_reg.h
 * @brief   IST8310 magnetomitor register table
 * @log     2018-04-15 nickelliang
 */

#ifndef _BSP_IST8310_REG_H_
#define _BSP_IST8310_REG_H_

/**
 * @ingroup bsp
 * @defgroup bsp_ist7310_reg BSP IST8310 REG
 * @{
 */

typedef enum {
    IST8310_WHO_AM_I           = 0x00,
    IST8310_ODR_MODE           = 0x01,
    IST8310_R_MODE             = 0x02,
    IST8310_R_XL               = 0x03,
    IST8310_R_XM               = 0x04,
    IST8310_R_YL               = 0x05,
    IST8310_R_YM               = 0x06,
    IST8310_R_ZL               = 0x07,
    IST8310_R_ZM               = 0x08,
    IST8310_R_CONFA            = 0x0A,
    IST8310_R_CONFB            = 0x0B,
    IST8310_ADDRESS            = 0x0E,
    IST8310_DEVICE_ID_A        = 0x00,  // Previously 0x10
    IST8310_AVGCNTL            = 0x41,
    IST8310_PDCNTL             = 0x42,
} IST8310_REG;

/** @} */

#endif
