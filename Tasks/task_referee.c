// /**
//  * @author  Nickel_Liang <nickelliang>
//  * @date    2018-04-21
//  * @file    task_referee.c
//  * @brief   RTOS task fro referee tx and rx
//  * @log     2018-04-21 nickelliang
//  */
//
// #include "task_referee.h"
//
// static osMutexId referee_rx_mutex;
// static osMutexId referee_tx_mutex;
// static osEvent referee_event;
// static data_process_t referee_process;
//
// referee_t referee_info;
// TaskHandle_t referee_task_handle;
//
// uint8_t referee_task_init(void) {
//     /* Create rx mutex */
//     osMutexDef(referee_rx_mutex);
//     referee_rx_mutex = osMutexCreate(osMutex(referee_rx_mutex));
//     /* Create tx mutex */
//     /* @note unused for now */
//     osMutexDef(referee_tx_mutex);
//     referee_tx_mutex = osMutexCreate(osMutex(referee_tx_mutex));
//     /* Initialize data process instance */
//     &referee_process = data_process_init(&REFEREE_PORT, referee_rx_mutex, REFEREE_FIFO_SIZE, REFEREE_BUFF_SIZE, REFEREE_SOF, &referee_dispatcher, &referee_info);
//     if (NULL == referee_process) {
//         bsp_error_handler(__FILE__, __LINE__, "Referee data process instance initialization failed.");
//         return 0;
//     }
//     if (!referee_init(&referee_process)) {
//         bsp_error_handler(__FILE__, __LINE__, "Referee UART hardware initialization failed.");
//         return 0;
//     }
//     return 1;
// }
//
// void referee_task(void const *argu) {
//     while (!referee_task_init()) {
//         bsp_error_handler(__FILE__, __LINE__, "Referee task failed to init.");
//     }
//     while (1) {
//         referee_event = osSignalWait(REFEREE_RX_SIGNAL | REFEREE_TX_SIGNAL, osWaitForever);
//     }
// }
//
// event = osSignalWait(JUDGE_UART_TX_SIGNAL | \
//                          JUDGE_UART_IDLE_SIGNAL, osWaitForever);
//
//     if (event.status == osEventSignal)
//     {
//       //receive judge data puts fifo
//       if (event.value.signals & JUDGE_UART_IDLE_SIGNAL)
//       {
//         dma_buffer_to_unpack_buffer(&judge_rx_obj, UART_IDLE_IT);
//         unpack_fifo_data(&judge_unpack_obj, DN_REG_ID);
//       }
//
//       //send data to judge system
//       if (event.value.signals & JUDGE_UART_TX_SIGNAL)
//       {
//         send_packed_fifo_data(&judge_txdata_fifo, DN_REG_ID);
//       }
//
//     }
//
//
// /**
//  * Callback function declared in bsp_uart. This is a weak function.
//  *
//  * @author Nickel_Liang
//  * @date   2018-04-18
//  */
// void uart_referee_callback(void) {
//     /* @todo Signal handling here */
//     return;
// }
