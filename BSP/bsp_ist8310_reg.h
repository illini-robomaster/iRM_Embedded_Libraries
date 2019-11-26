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

/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-15
 * @file    bsp_ist8310_reg.h
 * @brief   IST8310 magnetomitor register table
 * @log     2018-04-15 nickelliang
 */

#ifndef _BSP_IST8310_REG_H_
#define _BSP_IST8310_REG_H_

#ifdef __cplusplus
extern "C" {
#endif

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
    IST8310_DEVICE_ID_A        = 0x10,  // Previously 0x10
    IST8310_AVGCNTL            = 0x41,
    IST8310_PDCNTL             = 0x42,
} IST8310_REG;

/** @} */

#ifdef __cplusplus
}
#endif

#endif
