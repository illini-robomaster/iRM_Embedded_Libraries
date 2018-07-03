/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-30
 * @file    test_oled_module.h
 * @brief   Test RM2018 OLED module
 * @log     2018-05-30 nickelliang
 */

#ifndef _TEST_OLED_MODULE_H_
#define _TEST_OLED_MODULE_H_

#include "oled_module.h"
#include "bsp_error_handler.h"

uint8_t test_oled_module(void);

void test_oled_button_raw(void);

void test_oled_button(void);

void test_oled_display(void);

#endif
