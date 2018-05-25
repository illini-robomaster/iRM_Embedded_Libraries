/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-22
 * @file    task_tx2.c
 * @brief   RTOS task for TX2(PC) tx and rx
 * @log     2018-05-22 nickelliang
 */

#include "task_tx2.h"

static osMutexId        tx2_rx_mutex;
static osEvent          tx2_event;
static data_process_t*  tx2_process;

tx2_t       tx2_info;
osThreadId  tx2_task_handle;

void tx2_task_create(void) {
    osThreadDef(tx2_task_name, tx2_task, osPriorityAboveNormal, 0, 512);
    tx2_task_handle = osThreadCreate(osThread(tx2_task_name), NULL);
#ifdef DEBUG
    BSP_DEBUG;
    if (tx2_task_handle == NULL)
        print("TX2 task create failed.\r\n");
    else
        print("TX2 task created.\r\n");
#endif
}

uint8_t tx2_task_init(void) {
    /* Create rx mutex */
    osMutexDef(tx2_rx_mutex);
    tx2_rx_mutex = osMutexCreate(osMutex(tx2_rx_mutex));
    /* Initialize data process instance */
    tx2_process = data_process_init(&TX2_PORT, tx2_rx_mutex, TX2_FIFO_SIZE, TX2_BUFF_SIZE, TX2_SOF, tx2_dispatcher, &tx2_info);
    if (NULL == tx2_process) {
        bsp_error_handler(__FUNCTION__, __LINE__, "TX2 data process instance initialization failed.");
        return 0;
    }
    if (!tx2_init(tx2_process)) {
        bsp_error_handler(__FUNCTION__, __LINE__, "TX2 UART hardware initialization failed.");
        return 0;
    }
    return 1;
}

void tx2_task(void const *argu) {
    while (!tx2_task_init()) {
        bsp_error_handler(__FUNCTION__, __LINE__, "TX2 task failed to init.");
    }
#ifdef DEBUG
    BSP_DEBUG;
    print("TX2 task initialized.\r\n");
#endif
    while (1) {
#ifdef DEBUG
        BSP_DEBUG;
        print("In tx2 task loop.\r\n");
#endif
        tx2_event = osSignalWait(TX2_RX_SIGNAL | TX2_TX_SIGNAL, osWaitForever);
#ifdef DEBUG
        BSP_DEBUG;
        print("After wait forever.\r\n");
#endif
        if (tx2_event.status == osEventSignal) {
            if (tx2_event.value.signals & TX2_RX_SIGNAL) {
#ifdef DEBUG
                BSP_DEBUG;
                print("Rx signal received.\r\n");
#endif
                data_process_rx(tx2_process);
            }
            if (tx2_event.value.signals & TX2_TX_SIGNAL) {
                /* @note Not yet implemented */
                print("Tx signal received.\r\n");
            }
        }
    }
}

/**
 * Callback function declared in tx2.c. This is a weak function.
 *
 * @author Nickel_Liang
 * @date   2018-05-23
 */
void tx2_callback() {
    osSignalSet(tx2_task_handle, TX2_RX_SIGNAL);
}
