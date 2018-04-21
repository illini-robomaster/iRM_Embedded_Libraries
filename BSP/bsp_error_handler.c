/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-16
 * @file    bsp_error_handler.c
 * @brief   Error handler used by bsp files
 * @log     2018-04-16 nickelliang
 */

#include "bsp_error_handler.h"

void bsp_error_handler(const char* func, int line, char* msg) {
    print("[ERROR at ");
    print("%s:", func);
    print("%d] ", line);
    print("%s\r\n", msg);
    return;
}
