/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-23
 * @file    bsp_sdio.h
 * @brief   Board support package for SD card.
 * @log     2018-05-23 nickelliang
 */

#ifndef _BSP_SDIO_H_
#define _BSP_SDIO_H_

// #include "bsp_driver_sd.h"
// #include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "sdio.h"
// #include "main.h"
#include "sd_diskio.h"

void sdio_init(void);
void sdio_queue_init(void);

#endif
