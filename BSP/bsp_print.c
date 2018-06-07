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
