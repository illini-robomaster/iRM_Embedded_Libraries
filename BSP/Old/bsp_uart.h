/******************************************************************************
	Description
		UART BSP
    Log
		12/06/17 Nickel Liang	First Draft
		03/05/18 Alvin Sun      Add protocol.h inclusion
*******************************************************************************/

#ifndef __BSP_UART
#define __BSP_UART

#include "mytype.h"
#include "dbus.h"
#include "protocol.h"

#endif

//
// /******************************************************************************
// 	Description
// 		Robomaster communication protocol (included header files might should be
//                                       changed )
//     Log
// 		Mar-3-2018 Mingda Ma First Draft
//     Mar-4-2018 Mingda Ma Keep Editing
// *******************************************************************************/
// //
// // part1:revised from RM_ECE/Reference/RoboRTS-Firmware/MDK-ARM/rmlib/protocol.h
// //
// #ifndef __PROTOCOL_H__
// #define __PROTOCOL_H__
//
// #include "stm32f4xx_hal.h"
//
// #define UP_REG_ID    0xA0  //up layer regional id
// #define DN_REG_ID    0xA5  //down layer regional id
// #define HEADER_LEN   sizeof(frame_header_t)
// #define CMD_LEN      2    //cmdid bytes
// #define CRC_LEN      2    //crc16 bytes
//
// #define PROTOCAL_FRAME_MAX_SIZE  200
//
// /**
//   * @brief  frame header structure definition
//   */
//
// typedef __packed struct
// {
//   uint32_t  irm;          /* 域ID, 主控和裁判系统域：0xA5；主控和PC上层域：0xA0 */
//   uint16_t data_length;   /* 每帧内数据data的长度 */
//   uint8_t  crc8;          /* 帧头的crc校验结果 */
// } frame_header_t;
//
// uint8_t verify_crc8_check_sum(uint8_t* pchMessage, uint16_t dwLength);
// uint8_t verify_crc16_check_sum(uint8_t* pchMessage, uint32_t dwLength);
//
// void append_crc8_check_sum(uint8_t* pchMessage, uint16_t dwLength);
// void append_crc16_check_sum(uint8_t* pchMessage, uint32_t dwLength);
//
//
// #endif
//
// //
// // part2: revised from .../RoboRTS-Firmware/MDK-ARM/rmlib/judgement_info.h
// // to be edited: 0x0100, 0x0101, 0x0102; student custom info;
// // judgement information handle
// //
// #ifndef __JUDGEMENT_INFO_H__
// #define __JUDGEMENT_INFO_H__
//
// #include "stm32f4xx_hal.h"
//
// #define JUDGE_FRAME_BUFLEN 200
//
// /**
//   * @brief  judgement data command id
//   */
// typedef enum
// {
//   GAME_INFO_ID       = 0x0001,  //10Hz
//   REAL_FIELD_DATA_ID = 0x0005,  //10hZ
//   GAIN_BUFF_ID       = 0x0007,
//
//   STU_CUSTOM_DATA_ID = 0x0100,
//   ROBOT_TO_CLIENT_ID = 0x0101,
//   CLIENT_TO_ROBOT_ID = 0x0102,
// } judge_data_id_e;
//
// /**
//   * @brief  game information structures definition(0x0001)
//   *         this package send frequency is 50Hz
//   */
// typedef __packed struct     /* 0x0001 比赛进程 */
// {                           /* 对应数据结构 game_info_t，比赛进程信息 */
//   uint16_t   stage_remain_time;     /* 当前阶段剩余时间，单位 s */
//   uint8_t    game_process;
//   /* current race stage
//      0 not start
//      1 preparation stage
//      2 self-check stage
//      3 5 seconds count down
//      4 fighting stage
//      5 result computing stage */
//   uint8_t    reserved;
//   uint16_t   remain_hp;
//   uint16_t   max_hp;
//   position_t position;
// } game_robot_state_t;
//
// /**
//   * @brief  GPS state structures definition
//   */
// typedef __packed struct
// {
//   uint8_t valid_flag;	/* 位置、角度信息有效标志位 */
//   float x;
//   float y;
//   float z;
//   float yaw;
// } position_t;
//
// /**
//   * @brief  rfid detect data(0x0005)
//   */
//   typedef __packed struct
//   {
//     uint8_t region_idx;
//   } rfid_detect_t;
//
// /**
//   * @brief  the data of get field buff(0x0007)
//   */
// typedef __packed struct
// {
//   uint8_t buff_type;
//   uint8_t buff_addition;
// } get_buff_t;
//
// /**
//   * @brief  student custom data
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
//   game_robot_state_t game_information;
//   robot_hurt_data_t  blood_changed_data;
//   real_shoot_t       real_shoot_data;
//   rfid_detect_t      rfid_data;
//   game_result_t      game_result_data;
//   get_buff_t         get_buff_data;
//   server_to_user_t   student_download_data;
// } receive_judge_t;
//
// /* data send (forward) */
// /* data receive */
// extern receive_judge_t judge_rece_mesg;
//
// void  judgement_data_handle(uint8_t *p_frame);
//
// #endif
//
// //
// // part3: revised from .../RoboRTS-Firmware/MDK-ARM/rmlib/infantry_info.h
// // to be edited: 0x00A1, 0x00A3; send_pc_t and receive_pc_t;
// // to be added : 0x00A1, 0x00A2, 0x00A5 (document not completed yet)
// //
// #ifndef __INFANTRY_INFO_H__
// #define __INFANTRY_INFO_H__
//
// #include "stm32f4xx_hal.h"
// #include "judgement_info.h"
//
// #define COMPUTER_FRAME_BUFLEN 200
//
// /**
//   * @brief  infantry robot data command id
//   */
// typedef enum
// {
//   SHOOT_TASK_DATA_ID  = 0x0012,
//   INFANTRY_ERR_ID     = 0x0013,
//
//   CHASSIS_CTRL_ID     = 0x00A0,
//   GIMBAL_CTRL_ID      = 0x00A1,
//   SHOOT_CTRL_ID       = 0x00A2,
//   ERROR_LEVEL_ID      = 0x00A3,
//   CALI_GIMBAL_ID      = 0x00A5,
// } infantry_data_id_e;
//
// typedef enum
// {
//   BOTTOM_DEVICE        = 0,
//   GIMBAL_GYRO_OFFLINE  = 1,
//   CHASSIS_GYRO_OFFLINE = 2,
//   CHASSIS_M1_OFFLINE   = 3,
//   CHASSIS_M2_OFFLINE   = 4,
//   CHASSIS_M3_OFFLINE   = 5,
//   CHASSIS_M4_OFFLINE   = 6,
//   REMOTE_CTRL_OFFLINE  = 7,
//   JUDGE_SYS_OFFLINE    = 8,
//   PC_SYS_OFFLINE       = 9,
//   GIMBAL_YAW_OFFLINE   = 10,
//   GIMBAL_PIT_OFFLINE   = 11,
//   TRIGGER_MOTO_OFFLINE = 12,
//   BULLET_JAM           = 13,
//   CHASSIS_CONFIG_ERR   = 14,
//   GIMBAL_CONFIG_ERR    = 15,
//   ERROR_LIST_LENGTH    = 16,
// } err_id_e;
//
// typedef enum
// {
//   DEVICE_NORMAL = 0,
//   ERROR_EXIST   = 1,
//   UNKNOWN_STATE = 2,
// } bottom_err_e;
//
// typedef enum
// {
//   GLOBAL_NORMAL        = 0,
//   SOFTWARE_WARNING     = 1,
//   SOFTWARE_ERROR       = 2,
//   SOFTWARE_FATAL_ERROR = 3,
//   GIMBAL_ERROR         = 4,
//   CHASSIS_ERROR        = 5,
//   HARAWARE_ERROR       = 6,
// } err_level_e;
//
// /**
//   * @brief  shoot information
//   */
// typedef __packed struct
// {
//   int16_t remain_bullets;  /* the member of remain bullets */
//   int16_t shot_bullets;    /* the member of bullets that have been shot */
//   uint8_t fric_wheel_run;  /* friction run or not */
// } shoot_info_t;
//
// /**
//   * @brief  gimbal control information
//   */
// typedef __packed struct
// {
//   float   pit_ref;      /* gimbal pitch reference angle(degree) */
//   float   yaw_ref;      /* gimbal yaw reference angle(degree) */
// } gimbal_ctrl_t;
//
// /**
//   * @brief  robot system error level
//   */
// typedef __packed struct
// {
//   err_level_e err_level; /* the error level is included in err_level_e enumeration */
// } global_err_level_t;
//
// /********* variables **********/
// /**
//   * @brief  the data structure send to pc
//   */
// typedef struct
// {
//   /* data send */
//   chassis_info_t    chassis_information;
//   gimbal_info_t     gimbal_information;
//   shoot_info_t      shoot_task_information;
//   infantry_err_t    bottom_error_data;
//   config_response_t structure_config_data;
//   cali_response_t   cali_response_data;
//   rc_info_t         remote_ctrl_data;
//   version_info_t    version_info_data;
// } send_pc_t;
//
// /**
//   * @brief  the data structure receive from pc
//   */
// typedef struct
// {
//   /* data receive */
//   chassis_ctrl_t       chassis_control_data;
//   gimbal_ctrl_t        gimbal_control_data;
//   shoot_ctrl_t         shoot_control_data;
//   global_err_level_t   global_error_level;
//   infantry_structure_t structure_data;
//   cali_cmd_t           cali_cmd_data;
//   /* receive to forward */
//   client_show_data_t   show_in_client_data;
//   user_to_server_t     pc_to_server_data;
// } receive_pc_t;
//
// /* data send */
// extern send_pc_t    pc_send_mesg;
// /* data receive */
// extern receive_pc_t pc_rece_mesg;
//
// void pc_data_handle(uint8_t *p_frame);
//
// #endif
//
// //
// // part4: revised from .../RoboRTS-Firmware/MDK-ARM/rmlib/communicate.h
// // Only added an enumeration can be found in the document
// // To be added || To be Edited: Too much;
// // Mayday!! Mayday!!
// //
// #ifndef __COMMUNICATE_H__
// #define __COMMUNICATE_H__
//
// #include "stm32f4xx_hal.h"
// #include "data_fifo.h"
// #include "protocol.h"
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
