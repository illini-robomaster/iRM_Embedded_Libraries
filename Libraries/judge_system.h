// /****************************************************************************
//  *  Copyright (C) 2018 RoboMaster.
//  *
//  *  This program is free software: you can redistribute it and/or modify
//  *  it under the terms of the GNU General Public License as published by
//  *  the Free Software Foundation, either version 3 of the License, or
//  *  (at your option) any later version.
//  *
//  *  This program is distributed in the hope that it will be useful,
//  *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  *  GNU General Public License for more details.
//  *
//  *  You should have received a copy of the GNU General Public License
//  *  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  ***************************************************************************/
// /** @file judgement_info.h
//  *  @version 1.0
//  *  @date June 2017
//  *
//  *  @brief the information from judgement system
//  *
//  *  @copyright 2017 DJI RoboMaster. All rights reserved.
//  *
//  */
//
// #ifndef __JUDGE_SYSTEM_H__
// #define __JUDGE_SYSTEM_H__
//
// #include "stm32f4xx_hal.h"
// #include "data_fifo.h"
// #include "referee_crc.h"
//
// #define JUDGE_FIFO_BUFLEN 500
// #define COMPUTER_FIFO_BUFLEN 500
//
// #define UART_RX_DMA_SIZE       1024
//
// #define DBUS_HUART         huart1 //for dji remote controler reciever
// #define JUDGE_HUART        huart3 //connected to judge system
// #define COMPUTER_HUART     huart6 //connected to manifold/TXone
//
// /**
//   * @brief  judgement data command id
//   */
// typedef enum
// {
//   GAME_INFO_ID       = 0x0001,
//   REAL_BLOOD_DATA_ID = 0x0002,
//   REAL_SHOOT_DATA_ID = 0x0003,
//   REAL_POWER_DATA_ID = 0x0004,
//   FIELD_RFID_DATA_ID = 0x0005,
//   GAME_RESULT_ID     = 0x0006,
//   GAIN_BUFF_ID       = 0x0007,
//   ROBOT_POS_DATA_ID  = 0x0008,
//
//   STU_CUSTOM_DATA_ID = 0x0100,
//   ROBOT_TO_CLIENT_ID = 0x0101,
//   CLIENT_TO_ROBOT_ID = 0x0102,
// } judge_data_id_e;
//
//
// /**
//   * @brief  game information structures definition(0x0001)
//   */
// typedef __packed struct
// {
//   uint16_t   stage_remain_time;
//   uint8_t    game_process;
//   /* current race stage
//    0 not start
//    1 preparation stage
//    2 self-check stage
//    3 5 seconds count down
//    4 fighting stage
//    5 result computing stage */
//   uint8_t    reserved;
//   uint16_t   remain_hp;
//   uint16_t   max_hp;
// } game_robot_state_t;
//
// /**
//   * @brief  real time blood volume change data(0x0002)
//   */
// typedef __packed struct
// {
//   uint8_t armor_type:4;
//  /* 0-3bits: the attacked armor id:
//     0x00: 0 front
//     0x01：1 left
//     0x02：2 behind
//     0x03：3 right
//     others reserved*/
//   uint8_t hurt_type:4;
//  /* 4-7bits: blood volume change type
//     0x00: armor attacked
//     0x01：module offline
//     0x02: bullet over speed
//     0x03: bullet over frequency */
// } robot_hurt_data_t;
//
// /**
//   * @brief  real time shooting data(0x0003)
//   */
// typedef __packed struct
// {
//   uint8_t bullet_type;
//   uint8_t bullet_freq;
//   float   bullet_spd;
// } real_shoot_t;
//
// /**
//   * @brief  real chassis power and shoot heat data(0x0004)
//   *         icra need not this data
//   */
// typedef __packed struct
// {
//   float chassis_volt;
//   float chassis_current;
//   float chassis_power;
//   float chassis_pwr_buf;
//   uint16_t shooter1_heat;
//   uint16_t shooter2_heat;
// } real_power_data_t;
//
// /**
//   * @brief  field rfid data(0x0005)
//   */
// typedef __packed struct
// {
//   uint8_t card_type;
//   uint8_t card_idx;
// } field_rfid_t;
//
// /**
//   * @brief  game result data(0x0006)
//   */
// typedef __packed struct
// {
//   uint8_t winner;
// } game_result_t;
//
// /**
//   * @brief  the data of get field buff(0x0007)
//   */
// typedef __packed struct
// {
//   uint16_t buff_musk;
// } get_buff_t;
//
// /**
//   * @brief  field UWB data(0x0008)
//   */
// typedef __packed struct
// {
//   float x;
//   float y;
//   float z;
//   float yaw;
// } robot_position_t;
//
// /**
//   * @brief  student custom data
//   *         icra need not these data
//   */
// typedef __packed struct
// {
//   float data1;
//   float data2;
//   float data3;
// } client_show_data_t;
//
// typedef __packed struct
// {
//   uint8_t  data[64];
// } user_to_server_t;
//
// typedef __packed struct
// {
//   uint8_t  data[32];
// } server_to_user_t;
//
// /**
//   * @brief  the data structure receive from judgement
//   */
// typedef struct
// {
//   game_robot_state_t game_information;   //0x0001
//   robot_hurt_data_t  blood_changed_data; //0x0002
//   real_shoot_t       real_shoot_data;    //0x0003
//   real_power_data_t  power_heat_data;    //0x0004
//   field_rfid_t       rfid_data;          //0x0005
//   game_result_t      game_result_data;   //0x0006
//   get_buff_t         get_buff_data;      //0x0007
//   robot_position_t   robot_pos_data;     //0x0008
// } receive_judge_t;
//
// typedef enum
// {
//   STEP_HEADER_SOF  = 0,
//   STEP_LENGTH_LOW  = 1,
//   STEP_LENGTH_HIGH = 2,
//   STEP_FRAME_SEQ   = 3,
//   STEP_HEADER_CRC8 = 4,
//   STEP_DATA_CRC16  = 5,
// } unpack_step_e;
//
// typedef enum
// {
//   UART_IDLE_IT     = 0,
//   UART_DMA_HALF_IT = 1,
//   UART_DMA_FULL_IT = 2,
// } uart_it_type_e;
//
// typedef struct
// {
//   UART_HandleTypeDef *huart;
//   fifo_s_t           *data_fifo;
//   uint16_t           buff_size;
//   uint8_t            *buff[2];
//   uint16_t           read_index;
//   uint16_t           write_index;
// } uart_dma_rxdata_t;
//
// typedef struct
// {
//   fifo_s_t       *data_fifo;
//   frame_header_t *p_header;
//   uint16_t       data_len;
//   uint8_t        protocol_packet[PROTOCAL_FRAME_MAX_SIZE];
//   unpack_step_e  unpack_step;
//   uint16_t       index;
// } unpack_data_t;
//
// /* data send (forward) */
// /* data receive */
// extern receive_judge_t judge_rece_mesg;
//
// void judge_unpack_task(void const *argu);
// void unpack_fifo_data(unpack_data_t *p_obj, uint8_t sof);
// void judgement_data_handler(uint8_t *p_frame);
// void dma_buffer_to_unpack_buffer(uart_dma_rxdata_t *dma_obj, uart_it_type_e it_type);
// void get_dma_memory_msg(DMA_Stream_TypeDef *dma_stream, uint8_t *mem_id, uint16_t *remain_cnt);
// uint8_t dma_current_memory_target(DMA_Stream_TypeDef *dma_stream);
// uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream);
// static HAL_StatusTypeDef DMAEx_MultiBufferStart_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength);
// void judgement_uart_init(void);
//
// static void dma_m1_rxcplt_callback(DMA_HandleTypeDef *hdma);
// static void dma_m0_rxcplt_callback(DMA_HandleTypeDef *hdma);
// void uart_dma_full_signal(UART_HandleTypeDef *huart);
// #endif
