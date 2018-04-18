// /******************************************************************************
// 	Description
// 		JUDGE SYSTEM Utility
// 	Log
// 		01/21/18 Nickel Liang	First Draft
// *******************************************************************************/
//
// #include "judge_system.h"
//
// receive_judge_t judge_rece_mesg;
//
// /* communicate task static parameter */
// /* judge system receive data fifo and buffer*/
// // static osMutexId judge_rxdata_mutex;
// // static fifo_s_t  judge_rxdata_fifo;
// // static uint8_t   pc_rxdata_buf[COMPUTER_FIFO_BUFLEN];
// /* judge system send data fifo and buffer*/
// // static osMutexId judge_txdata_mutex;
// // static fifo_s_t  judge_txdata_fifo;
// // static uint8_t   pc_txdata_buf[COMPUTER_FIFO_BUFLEN];
// /* judge system dma receive data object */
// static uart_dma_rxdata_t judge_rx_obj;
// /* pc receive data fifo and buffer */
// // static osMutexId pc_rxdata_mutex;
// // static fifo_s_t  pc_rxdata_fifo;
// // static uint8_t   judge_rxdata_buf[JUDGE_FIFO_BUFLEN];
// /* pc send data fifo and buffer */
// // static osMutexId pc_txdata_mutex;
// // static fifo_s_t  pc_txdata_fifo;
// // static uint8_t   judge_txdata_buf[JUDGE_FIFO_BUFLEN];
// /* pc system dma receive data object */
// // static uart_dma_rxdata_t pc_rx_obj;
// /* unpack object */
// static unpack_data_t judge_unpack_obj;
// // static unpack_data_t pc_unpack_obj;
//
// uint8_t judge_dma_rxbuff[2][UART_RX_DMA_SIZE];
//
// void judge_unpack_task(void const *argu) {
//     judgement_uart_init();
//     while(1) {
//         dma_buffer_to_unpack_buffer(&judge_rx_obj, UART_IDLE_IT);
//         unpack_fifo_data(&judge_unpack_obj, DN_REG_ID);
//     }
// }
//
// void unpack_fifo_data(unpack_data_t *p_obj, uint8_t sof) {
//     uint8_t byte = 0;
//
//   while ( fifo_used_count(p_obj->data_fifo) ){
//     byte = fifo_s_get(p_obj->data_fifo);
//     switch(p_obj->unpack_step)
//     {
//       case STEP_HEADER_SOF:
//       {
//         if(byte == sof)
//         {
//           p_obj->unpack_step = STEP_LENGTH_LOW;
//           p_obj->protocol_packet[p_obj->index++] = byte;
//         }
//         else
//         {
//           p_obj->index = 0;
//         }
//       }break;
//
//       case STEP_LENGTH_LOW:
//       {
//         p_obj->data_len = byte;
//         p_obj->protocol_packet[p_obj->index++] = byte;
//         p_obj->unpack_step = STEP_LENGTH_HIGH;
//       }break;
//
//       case STEP_LENGTH_HIGH:
//       {
//         p_obj->data_len |= (byte << 8);
//         p_obj->protocol_packet[p_obj->index++] = byte;
//
//         if(p_obj->data_len < (PROTOCAL_FRAME_MAX_SIZE - HEADER_LEN - CRC_LEN))
//         {
//           p_obj->unpack_step = STEP_FRAME_SEQ;
//         }
//         else
//         {
//           p_obj->unpack_step = STEP_HEADER_SOF;
//           p_obj->index = 0;
//         }
//       }break;
//
//       case STEP_FRAME_SEQ:
//       {
//         p_obj->protocol_packet[p_obj->index++] = byte;
//         p_obj->unpack_step = STEP_HEADER_CRC8;
//       }break;
//
//       case STEP_HEADER_CRC8:
//       {
//         p_obj->protocol_packet[p_obj->index++] = byte;
//
//         if (p_obj->index == HEADER_LEN)
//         {
//           if ( verify_crc8_check_sum(p_obj->protocol_packet, HEADER_LEN) )
//           {
//             p_obj->unpack_step = STEP_DATA_CRC16;
//           }
//           else
//           {
//             p_obj->unpack_step = STEP_HEADER_SOF;
//             p_obj->index = 0;
//           }
//         }
//       }break;
//
//       case STEP_DATA_CRC16:
//       {
//         if (p_obj->index < (HEADER_LEN + CMD_LEN + p_obj->data_len + CRC_LEN))
//         {
//            p_obj->protocol_packet[p_obj->index++] = byte;
//         }
//         if (p_obj->index >= (HEADER_LEN + CMD_LEN + p_obj->data_len + CRC_LEN))
//         {
//           p_obj->unpack_step = STEP_HEADER_SOF;
//           p_obj->index = 0;
//
//           if ( verify_crc16_check_sum(p_obj->protocol_packet, HEADER_LEN + CMD_LEN + p_obj->data_len + CRC_LEN) )
//           {
//             if (sof == UP_REG_ID)
//             {
//               // pc_data_handler(p_obj->protocol_packet);
//             }
//             else  //DN_REG_ID
//             {
//               judgement_data_handler(p_obj->protocol_packet);
//             }
//           }
//         }
//       }break;
//
//       default:
//       {
//         p_obj->unpack_step = STEP_HEADER_SOF;
//         p_obj->index = 0;
//       }break;
//     }
//   }
// }
//
// // Used by unpack_fifo_data
// void judgement_data_handler(uint8_t *p_frame) {
//     frame_header_t *p_header = (frame_header_t*)p_frame;
//     memcpy(p_header, p_frame, HEADER_LEN);
//
//     uint16_t data_length = p_header->data_length;
//     uint16_t cmd_id      = *(uint16_t *)(p_frame + HEADER_LEN);
//     uint8_t *data_addr   = p_frame + HEADER_LEN + CMD_LEN;
//     // uint8_t  invalid_cmd = 0;
//
//     switch (cmd_id)
//     {
//     case GAME_INFO_ID:
//         memcpy(&judge_rece_mesg.game_information, data_addr, data_length);
//     break;
//
//     case REAL_BLOOD_DATA_ID:
//         memcpy(&judge_rece_mesg.blood_changed_data, data_addr, data_length);
//     break;
//
//     case REAL_SHOOT_DATA_ID:
//         memcpy(&judge_rece_mesg.real_shoot_data, data_addr, data_length);
//     break;
//
//     case FIELD_RFID_DATA_ID:
//         memcpy(&judge_rece_mesg.rfid_data, data_addr, data_length);
//     break;
//
//     case GAME_RESULT_ID:
//         memcpy(&judge_rece_mesg.game_result_data, data_addr, data_length);
//     break;
//
//     case GAIN_BUFF_ID:
//         memcpy(&judge_rece_mesg.get_buff_data, data_addr, data_length);
//     break;
//
//     case ROBOT_POS_DATA_ID:
//         memcpy(&judge_rece_mesg.robot_pos_data, data_addr, data_length);
//     break;
//
//     default:
//         // invalid_cmd = 1;
//     break;
//     }
// }
//
// // Used by judge_unpack_task
// int dma_write_len = 0;
// int fifo_overflow = 0;
// void dma_buffer_to_unpack_buffer(uart_dma_rxdata_t *dma_obj, uart_it_type_e it_type) {
//   int16_t  tmp_len;
//   uint8_t  current_memory_id;
//   uint16_t remain_data_counter;
//   uint8_t  *pdata = dma_obj->buff[0];
//
//   get_dma_memory_msg(dma_obj->huart->hdmarx->Instance, &current_memory_id, &remain_data_counter);
//
//   if (UART_IDLE_IT == it_type)
//   {
//     if (current_memory_id)
//     {
//       dma_obj->write_index = dma_obj->buff_size*2 - remain_data_counter;
//     }
//     else
//     {
//       dma_obj->write_index = dma_obj->buff_size - remain_data_counter;
//     }
//   }
//   else if (UART_DMA_FULL_IT == it_type)
//   {
//       #if 0
//     if (current_memory_id)
//     {
//       dma_obj->write_index = dma_obj->buff_size;
//     }
//     else
//     {
//       dma_obj->write_index = dma_obj->buff_size*2;
//     }
//     #endif
//   }
//
//   if (dma_obj->write_index < dma_obj->read_index)
//   {
//     dma_write_len = dma_obj->buff_size*2 - dma_obj->read_index + dma_obj->write_index;
//
//     tmp_len = dma_obj->buff_size*2 - dma_obj->read_index;
//     if (tmp_len != fifo_s_puts(dma_obj->data_fifo, &pdata[dma_obj->read_index], tmp_len))
//       fifo_overflow = 1;
//     else
//       fifo_overflow = 0;
//     dma_obj->read_index = 0;
//
//     tmp_len = dma_obj->write_index;
//     if (tmp_len != fifo_s_puts(dma_obj->data_fifo, &pdata[dma_obj->read_index], tmp_len))
//       fifo_overflow = 1;
//     else
//       fifo_overflow = 0;
//     dma_obj->read_index = dma_obj->write_index;
//   }
//   else
//   {
//     dma_write_len = dma_obj->write_index - dma_obj->read_index;
//
//     tmp_len = dma_obj->write_index - dma_obj->read_index;
//     if (tmp_len != fifo_s_puts(dma_obj->data_fifo, &pdata[dma_obj->read_index], tmp_len))
//       fifo_overflow = 1;
//     else
//       fifo_overflow = 0;
//     dma_obj->read_index = (dma_obj->write_index) % (dma_obj->buff_size*2);
//   }
// }
//
// // Used by dma_buffer_to_unpack_buffer
// void get_dma_memory_msg(DMA_Stream_TypeDef *dma_stream, uint8_t *mem_id, uint16_t *remain_cnt) {
//   *mem_id     = dma_current_memory_target(dma_stream);
//   *remain_cnt = dma_current_data_counter(dma_stream);
// }
//
// /**
//   * @brief  Returns the current memory target used by double buffer transfer.
//   * @param  dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
//   *          to 7 to select the DMA Stream.
//   * @retval The memory target number: 0 for Memory0 or 1 for Memory1.
//   */
// uint8_t dma_current_memory_target(DMA_Stream_TypeDef *dma_stream) {
//   uint8_t tmp = 0;
//
//   /* Get the current memory target */
//   if ((dma_stream->CR & DMA_SxCR_CT) != 0)
//   {
//     /* Current memory buffer used is Memory 1 */
//     tmp = 1;
//   }
//   else
//   {
//     /* Current memory buffer used is Memory 0 */
//     tmp = 0;
//   }
//   return tmp;
// }
//
// /**
//   * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer.
//   * @param  dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
//   *          to 7 to select the DMA Stream.
//   * @retval The number of remaining data units in the current DMAy Streamx transfer.
//   */
// uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream) {
//   /* Return the number of remaining data units for DMAy Streamx */
//   return ((uint16_t)(dma_stream->NDTR));
// }
//
// // Used by judge_unpack_task
// static HAL_StatusTypeDef DMAEx_MultiBufferStart_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength) {
//     HAL_StatusTypeDef status = HAL_OK;
//
//     /* Memory-to-memory transfer not supported in double buffering mode */
//     if (hdma->Init.Direction == DMA_MEMORY_TO_MEMORY) {
//         hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;
//         return HAL_ERROR;
//     }
//
//     /* Set the UART DMA transfer complete callback */
//     /* Current memory buffer used is Memory 1 callback */
//     hdma->XferCpltCallback   = dma_m0_rxcplt_callback;
//     /* Current memory buffer used is Memory 0 callback */
//     hdma->XferM1CpltCallback = dma_m1_rxcplt_callback;
//
//   /* Check callback functions */
//   if ((NULL == hdma->XferCpltCallback) || (NULL == hdma->XferM1CpltCallback))
//   {
//     hdma->ErrorCode = HAL_DMA_ERROR_PARAM;
//     return HAL_ERROR;
//   }
//
//   /* Process locked */
//   __HAL_LOCK(hdma);
//
//   if(HAL_DMA_STATE_READY == hdma->State)
//   {
//     /* Change DMA peripheral state */
//     hdma->State = HAL_DMA_STATE_BUSY;
//     /* Initialize the error code */
//     hdma->ErrorCode = HAL_DMA_ERROR_NONE;
//     /* Enable the Double buffer mode */
//     hdma->Instance->CR |= (uint32_t)DMA_SxCR_DBM;
//     /* Configure DMA Stream destination address */
//     hdma->Instance->M1AR = SecondMemAddress;
//
//     /* Configure DMA Stream data length */
//     hdma->Instance->NDTR = DataLength;
//     /* Configure the source, destination address */
//     if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
//     {
//       hdma->Instance->PAR = DstAddress;
//       hdma->Instance->M0AR = SrcAddress;
//     }
//     else
//     {
//       hdma->Instance->PAR = SrcAddress;
//       hdma->Instance->M0AR = DstAddress;
//     }
//
//     /* Clear TC flags */
//     __HAL_DMA_CLEAR_FLAG (hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));
//     /* Enable TC interrupts*/
//     hdma->Instance->CR  |= DMA_IT_TC;
//
//     /* Enable the peripheral */
//     __HAL_DMA_ENABLE(hdma);
//
//     /* Change the DMA state */
//     hdma->State = HAL_DMA_STATE_READY;
//   }
//   else
//   {
//     /* Return error status */
//     status = HAL_BUSY;
//   }
//
//   /* Process unlocked */
//   __HAL_UNLOCK(hdma);
//
//   return status;
// }
//
//
//
// // Used by judge_unpack_task
// void judgement_uart_init(void) {
//     //open uart idle it
//     __HAL_UART_CLEAR_IDLEFLAG(&JUDGE_HUART);
//     __HAL_UART_ENABLE_IT(&JUDGE_HUART, UART_IT_IDLE);
//
//     // Enable the DMA transfer for the receiver request
//     SET_BIT(JUDGE_HUART.Instance->CR3, USART_CR3_DMAR);
//
//     DMAEx_MultiBufferStart_IT(JUDGE_HUART.hdmarx, \
//                            (uint32_t)&JUDGE_HUART.Instance->DR, \
//                            (uint32_t)judge_dma_rxbuff[0], \
//                            (uint32_t)judge_dma_rxbuff[1], \
//                            UART_RX_DMA_SIZE);
// }
//
// static void dma_m1_rxcplt_callback(DMA_HandleTypeDef *hdma)
// {
//   UART_HandleTypeDef* huart = ( UART_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
//   uart_dma_full_signal(huart);
// }
//
// /* Current memory buffer used is Memory 1 */
// static void dma_m0_rxcplt_callback(DMA_HandleTypeDef *hdma)
// {
//   UART_HandleTypeDef* huart = ( UART_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
//   uart_dma_full_signal(huart);
// }
//
// void uart_dma_full_signal(UART_HandleTypeDef *huart)
// {
//   if (huart == &JUDGE_HUART)
//   {
//     /* remove DMA buffer full interrupt handler */
//     //osSignalSet(judge_unpack_task_t, JUDGE_DMA_FULL_SIGNAL);
//   }
//   // else if (huart == &COMPUTER_HUART)
//   // {
//     /* remove DMA buffer full interrupt handler */
//     //osSignalSet(pc_unpack_task_t, PC_DMA_FULL_SIGNAL);
//   // }
// }
