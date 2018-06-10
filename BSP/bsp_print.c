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

#include "bsp_print.h"
#include "cmsis_os.h"
#include <stdarg.h>
#include <stdlib.h>

void print(const char *fmt, ...) {
    if (PRINT_TO_UART == 1) {
        char *printf_temp = pvPortMalloc(512*sizeof(char));
        va_list args;
        va_start(args, fmt);
        vsprintf(printf_temp, fmt, args);
        va_end(args);
        HAL_UART_Transmit(&BSP_PRINT_PORT, (uint8_t*)printf_temp, strlen(printf_temp), 200);
        vPortFree(printf_temp);
    }
    if (PRINT_TO_SD == 1)
        return;
}
