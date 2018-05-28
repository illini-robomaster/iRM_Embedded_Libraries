/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-22
 * @file    task_test.c
 * @brief   RTOS task for test(PC) tx and rx
 * @log     2018-05-22 nickelliang
 */

#include "task_test.h"
#include "rm_test.h"

osThreadId      test_task_handle;

void test_task_create(void) {
    osThreadDef(test_task_name, test_task, osPriorityAboveNormal, 0, 512);
    test_task_handle = osThreadCreate(osThread(test_task_name), NULL);
    if (test_task_handle == NULL)
        bsp_error_handler(__FUNCTION__, __LINE__, "test task create failed.");
}

void test_task(void const *argu) {
    run_all_tests();
}

