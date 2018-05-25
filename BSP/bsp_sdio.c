/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-23
 * @file    bsp_sdio.c
 * @brief   Board support package for SD card.
 * @log     2018-05-23 nickelliang
 */

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
