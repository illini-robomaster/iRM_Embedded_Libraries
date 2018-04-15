//
// Created by Yixiao Sun on 20/12/2017.
//

#ifndef IRM2018_RM_PRINT_H
#define IRM2018_RM_PRINT_H



static char printf_temp[512];

#define RM_PRINTF(expr...) ({ sprintf(printf_temp, expr); HAL_UART_Transmit(&huart6, printf_temp, strlen(printf_temp), 2); })

#endif //IRM2018_RM_PRINT_H
