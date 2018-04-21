/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-16
 * @file    bsp_error_handler.c
 * @brief   Error handler used by bsp files
 * @log     2018-04-16 nickelliang
 */

#include "bsp_error_handler.h"

void bsp_error_handler(char* file, int line, char* msg) {
    print("[ERROR at %d]", line);
    print("%s", msg);
    return;
}
