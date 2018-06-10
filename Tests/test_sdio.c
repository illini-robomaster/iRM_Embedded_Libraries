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
 * @date    2018-05-24
 * @file    test_sdio.c
 * @brief   Unit test for sdio
 * @log     2018-05-24 nickelliang
 */

#include "test_sdio.h"

FATFS          SDFatFs;                               /* file system object for SD card logical drive */
static uint8_t buffer[_MAX_SS];                       /* a work buffer for the f_mkfs() */
uint32_t       byteswritten, bytesread;               /* file write/read counts */
uint8_t        rtext[100];                            /* file read buffer */
uint8_t        err;
static char    *f_name = "robomaster.txt";           /* file name */
uint8_t        wtext[] = " Welcome to Robomaster! "; /* file write buffer */

uint8_t test_sdio(void) {
    // sdio_init();
    // FRESULT ret;
    // /* Register the file system object to the FatFs module and create a FAT file system (format) on the logical drive */
    // ret = f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, buffer, sizeof(buffer));
    // if (ret == FR_OK) {
        print("SDIO initialize success.\r\n");
        if (f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK) {
            print("SDIO mount success.\r\n");
            /* Create and Open a new text file object with write access */
            if (f_open(&SDFile, f_name, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
                print("SDIO file create success.\r\n");
                /* Write data to the text file */
                f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten);
                /* Close the open text file */
                f_close(&SDFile);
                /* Open the text file object with read access */
                if (f_open(&SDFile, f_name, FA_READ) == FR_OK) {
                    print("SDIO file open success.\r\n");
                    /* Read data from the text file */
                    f_read(&SDFile, rtext, sizeof(rtext), (UINT*)&bytesread);
                    /* Close the open text file */
                    f_close(&SDFile);
                }
                else {
                    bsp_error_handler(__FUNCTION__, __LINE__, "Open file error!");
                    return 0;
                }
            }
            else {
                bsp_error_handler(__FUNCTION__, __LINE__, "Open file error!");
                return 0;
            }
        }
        else {
            bsp_error_handler(__FUNCTION__, __LINE__, "Register file system error or create file system error!");
            return 0;
        }
    // }
    // else {
    //     print("[ERROR Val] %x", ret);
    //     bsp_error_handler(__FUNCTION__, __LINE__, "SDIO initialize failed.");
    //     return 0;
    // }

    /* Unlink the SD disk I/O driver */
    // FATFS_UnLinkDriver(SDPath);
    return 1;
}
