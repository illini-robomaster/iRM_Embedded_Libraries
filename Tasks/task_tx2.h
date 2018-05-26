/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-22
 * @file    task_tx2.h
 * @brief   RTOS task for TX2(PC) tx and rx
 * @log     2018-05-22 nickelliang
 */

#ifndef _TASK_TX2_H_
#define _TASK_TX2_H_

#include "cmsis_os.h"
#include "tx2.h"

/* TX2 signal handling */
#define TX2_RX_SIGNAL   (1 << 2)
#define TX2_TX_SIGNAL   (1 << 3)

/* Global variable */
extern tx2_t        tx2_info;
extern osThreadId   tx2_task_handle;

/**
 * Create TX2 task
 *
 * @author Nickel_Liang
 * @date   2018-05-22
 */
void tx2_task_create(void);

/**
 * Initialize TX2 task software and hardware
 *
 * @return            1 for success, 0 for fail
 * @author Nickel_Liang
 * @date   2018-05-22
 */
uint8_t tx2_task_init(void);

/**
 * TX2 data processing task
 *
 * @param  argu       Passed in argument, usually un-used.
 * @author Nickel_Liang
 * @date   2018-05-22
 */
void tx2_task(void const *argu);

#endif
