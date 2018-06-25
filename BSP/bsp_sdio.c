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
 * @date    2018-05-23
 * @file    bsp_sdio.c
 * @brief   Board support package for SD card.
 * @log     2018-05-23 nickelliang
 */

#if 0

#include "bsp_sdio.h"
#include "cmsis_os.h"

osMessageQId SDQueueID;

void sdio_init(void) {
    BSP_SD_Init();
}

void sdio_queue_init(void) {
    osMessageQDef(SDQueueID_, 16, uint32_t);
    SDQueueID = osMessageCreate(osMessageQ(SDQueueID_), NULL);
}

// Weak function overwrite
// sd_diskio.h need to add:
// void BSP_SD_WriteCpltCallback();
// void BSP_SD_ReadCpltCallback();
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd) {
    BSP_SD_ReadCpltCallback();
}

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd) {
    BSP_SD_WriteCpltCallback();
}

// Check sd_diskio.c::SD_initialize(BYTE lun)
/*
  Stat = STA_NOINIT;
#if !defined(DISABLE_SD_INIT)
  if(BSP_SD_Init() == MSD_OK)
  {
    Stat = SD_CheckStatus(lun);
  }
#else
  Stat = SD_CheckStatus(lun);
#endif
  return Stat;
}
*/

#endif
