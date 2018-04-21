/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-21
 * @file    task_referee.c
 * @brief   RTOS task fro referee tx and rx
 * @log     2018-04-21 nickelliang
 */

#include "task_referee.h"

static osMutexId        referee_rx_mutex;
static osMutexId        referee_tx_mutex;
static osEvent          referee_event;
static data_process_t*  referee_process;

referee_t   referee_info;
osThreadId  referee_task_handle;

void referee_task_create(void) {
    osThreadDef(referee_task_name, referee_task, osPriorityNormal, 0, 512);
    referee_task_handle = osThreadCreate(osThread(referee_task_name), NULL);
}

uint8_t referee_task_init(void) {
    /* Create rx mutex */
    osMutexDef(referee_rx_mutex);
    referee_rx_mutex = osMutexCreate(osMutex(referee_rx_mutex));
    /* Create tx mutex */
    /* @note unused for now */
    osMutexDef(referee_tx_mutex);
    referee_tx_mutex = osMutexCreate(osMutex(referee_tx_mutex));
    /* Initialize data process instance */
    referee_process = data_process_init(&REFEREE_PORT, referee_rx_mutex, REFEREE_FIFO_SIZE, REFEREE_BUFF_SIZE, REFEREE_SOF, referee_dispatcher, &referee_info);
    if (NULL == referee_process) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Referee data process instance initialization failed.");
        return 0;
    }
    if (!referee_init(referee_process)) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Referee UART hardware initialization failed.");
        return 0;
    }
    return 1;
}

void referee_task(void const *argu) {
    while (!referee_task_init()) {
        bsp_error_handler(__FUNCTION__, __LINE__, "Referee task failed to init.");
    }
    while (1) {
        referee_event = osSignalWait(REFEREE_RX_SIGNAL | REFEREE_TX_SIGNAL, osWaitForever);
        if (referee_event.status == osEventSignal) {
            if (referee_event.value.signals & REFEREE_RX_SIGNAL)
                data_process_rx(referee_process);
            if (referee_event.value.signals & REFEREE_TX_SIGNAL)
                return; /* @note Not yet implemented */
        }
    }
}

/**
 * Callback function declared in bsp_uart. This is a weak function.
 *
 * @author Nickel_Liang
 * @date   2018-04-18
 */
void uart_referee_callback(void) {
    /* @todo Signal handling here */
    /* @todo second wrap for this function */
    osSignalSet(referee_task_handle, REFEREE_RX_SIGNAL);
    return;
}
