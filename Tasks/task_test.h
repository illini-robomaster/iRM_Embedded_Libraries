/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-22
 * @file    task_tx2.h
 * @brief   RTOS task for TX2(PC) tx and rx
 * @log     2018-05-22 nickelliang
 */

#ifndef _TASK_TEST_H_
#define _TASK_TEST_H_

#include "cmsis_os.h"

extern osThreadId       test_task_handle;

void test_task_create(void);

void test_task(void const *argu);

#endif
