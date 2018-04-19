/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-16
 * @file    bsp_uart.h
 * @brief   Board support package for uart device
 * @log     2018-04-17 nickelliang
 */

#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include "bsp_config.h"
#include "usart.h"
#include "dma.h"
#include "bsp_error_handler.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/**
 * @ingroup bsp
 * @defgroup bsp_uart BSP UART
 * @{
 */

/* ===== UART Utilities ===== */

/**
 * Start DMA transfer with interrupt disabled
 *
 * @param  huart      Which uart to set
 * @param  pData      Data stream buffer
 * @param  size       size of data
 * @return            0 for fail, 1 for success
 * @author Nickel_Liang
 * @date   2018-04-17
 * @note   Reference from HAL_UART_Receive_DMA
 */
uint8_t uart_rx_dma_without_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t size);

/**
 * Start DMS transfer in double buffer mode with interrupt enable
 *
 * @param  hdma             Which DMA stream to set
 * @param  SrcAddress       Source address
 * @param  DstAddress       Destination address
 * @param  SecondMemAddress Second memory addres
 * @param  DataLength       Data length
 * @return                  HAL status
 * @author Nickel_Liang
 * @date   2018-04-19
 * @note   Reference from DJI RTS-Firmware and hal_dma_ex.c
 */
uint8_t uart_dma_multibuffer_it(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength);

/**
 * Enable DMA transfer for rx
 *
 * @param  huart      Which UART to set
 * @author Nickel_Liang
 * @date   2018-04-19
 */
void uart_enable_receiver_dma(UART_HandleTypeDef* huart);

/**
 * Initialize a UART port with IDLE interrupt
 *
 * @param  huart      UART port to be initialize
 * @author Nickel_Liang
 * @date   2018-04-18
 */
void uart_port_init(UART_HandleTypeDef* huart);

/* ===== DMA Utilities ===== */

/**
 * Count how many data unit remain in DMA buffer
 *
 * @param  dma_stream Which DMA stream to count
 * @return            Number of bytes remain in DMA buffer
 * @author Nickel_Liang
 * @date   2018-04-17
 * @source https://blog.frankvh.com/2011/08/18/stm32f2xx-dma-controllers/
 */
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream);

/**
 * Check which memory target is in use by double buffer
 *
 * @param  dma_stream Which DMA stream to check
 * @return            0 for memory 0 in use, 1 for memory 1 in use
 * @author Nickel_Liang
 * @date   2018-04-19
 * @source http://www.chibios.com/forum/viewtopic.php?t=4137
 * @todo   Not tested yet
 */
uint8_t dma_current_memory_target(DMA_Stream_TypeDef *dma_stream);

/**
 * DMA M1 Callback function. Now filling M0.
 *
 * @param  hdma       Which DMA callback
 * @author Nickel_Liang
 * @date   2018-04-19
 */
static void dma_m1_rxcplt_callback(DMA_HandleTypeDef *hdma);

/**
 * DMA M0 Callback function. Now filling M1.
 *
 * @param  hdma       Which DMA callback
 * @author Nickel_Liang
 * @date   2018-04-19
 */
static void dma_m0_rxcplt_callback(DMA_HandleTypeDef *hdma);

/* ===== Weak Functions ===== */

/**
 * This is a weak function. Implement this function in dbus library.
 *
 * @author Nickel_Liang
 * @date   2018-04-18
 */
void uart_dbus_callback(void);

/** @} */

#endif
