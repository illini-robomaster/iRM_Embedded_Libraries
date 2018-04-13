/******************************************************************************
	Description
		JUDGE SYSTEM Utility
	Log
		01/21/18 Nickel Liang	First Draft
*******************************************************************************/

#ifndef __JUDGE_SYSTEM
#define __JUDGE_SYSTEM

#include "mytype.h"

/*Frame Header    5Byte
    SOF         1Byte       0xA5
    DataLength  2Byte       DataLength
    Seq         1Byte       Package Number
    CRC8        1Byte       CRC8 Check

CmdID           2Byte
    See Enum Below

Data
    See Struct Below

Frame Tail      2Byte
    CRC16
*/

/******************************************************************************/
/* Judge System Command ID */
typedef enum {
    JUDGE_Robot_Status  = 0x0001, // RM_JUDGE_Robot_Status_t
    JUDGE_Damage        = 0x0002, // RM_JUDGE_Damage_t
    JUDGE_Shoot         = 0x0003, // RM_JUDGE_Shoot_t
    JUDGE_Power_Heat    = 0x0004, // RM_JUDGE_Power_Heat_t
    JUDGE_RFID          = 0x0005, // RM_JUDGE_RFID_t
    JUDGE_Result        = 0x0006, // RM_JUDGE_Result_t
    JUDGE_Buff          = 0x0007, // RM_JUDGE_Buff_t
    JUDGE_Custom_Data   = 0x0100, // RM_JUDGE_Custom_Data_t
} RM_JUDGE_CMDID;

/******************************************************************************/
/* Robot Status Struct */
typedef struct {
    uint16_t remainTime;
    uint8_t gameProgress; // RM_JUDGE_Game_Progress
    uint8_t robotLevel;
    uint16_t remainHP;
    uint16_t maxHP;
    uint8_t positionValid; // RM_JUDGE_Position_Valid
    float x;
    float y;
    float z;
    float yaw;
} RM_JUDGE_Robot_Status_t; // 0x0001, 25 bytes

typedef enum {
    JUDGE_Game_Not_Started  = 0,
    JUDGE_Game_Preparing    = 1,
    JUDGE_Game_Self_Check   = 2,
    JUDGE_Game_Count_Down   = 3,
    JUDGE_Game_In_Game      = 4,
    JUDGE_Game_Result_Check = 5,
} RM_JUDGE_Game_Progress;

typedef enum {
    JUDGE_Position_Invalid  = 0,
    JUDGE_Position_Valid    = 1,
} RM_JUDGE_Position_Valid;

/******************************************************************************/
/* Damage Struct */
typedef struct {
    uint8_t armorType : 4; // RM_JUDGE_Armor_Type, Lower 4 bits
    uint8_t damageType : 4; // RM_JUDGE_Damage_Type, Higher 4 bits
} RM_JUDGE_Damage_t; // 0x0002, 1 byte

typedef enum {
    JUDGE_Armor_1           = 0x0, // Front
    JUDGE_Armor_2           = 0x1, // Left
    JUDGE_Armor_3           = 0x2, // Back
    JUDGE_Armor_4           = 0x3, // Right
    JUDGE_Armor_5           = 0x4, // Top1
    JUDGE_Armor_6           = 0x5, // Top2
} RM_JUDGE_Armor_Type;

typedef enum {
    JUDGE_Damage_Armor          = 0x1, // Normal Attack
    JUDGE_Damage_Module_Offline = 0x2, // Judge System Module Offline
    JUDGE_Damage_Over_Speed     = 0x3, // Shooting System Over Speed
    JUDGE_Damage_Over_Frequency = 0x4, // Shooting System Over Frequency
    JUDGE_Damage_Over_Heat      = 0x5, // Shooting System Over Heat
    JUDGE_Damage_Over_Power     = 0x6, // Chassis Over Power
} RM_JUDGE_Damage_Type;

/******************************************************************************/
/* Shoot Struct */
typedef struct {
    uint8_t bulletType; // RM_JUDGE_Bullet_Type
    uint8_t bulletFreq;
    float bulletSpeed;
    float reserved;
} RM_JUDGE_Shoot_t; // 0x0003, 10 byte

typedef enum {
    JUDGE_Bullet_17mm       = 1,
    JUDGE_Bullet_42mm       = 2,
} RM_JUDGE_Bullet_Type;

/******************************************************************************/
/* Power & Heat Struct */
typedef struct {
    float chassisVolt;
    float chassisCurrent;
    float chassisPower;
    float chassisPowerBuffer;
    uint16_t shootHeat17;
    uint16_t shootHeat42;
} RM_JUDGE_Power_Heat_t; // 0x0004, 20 byte

/******************************************************************************/
/* RFID Struct */
typedef struct {
    uint8_t cardType; // RM_JUDGE_Card_Type
    uint8_t cardIdx;
} RM_JUDGE_RFID_t; // 0x0005, 2 byte

typedef enum {
    JUDGE_Card_Attack_Buff  = 0,
    JUDGE_Card_Defense_Buff = 1,
    JUDGE_Card_Red_Healing  = 2,
    JUDGE_Card_Blue_Healing = 3,
    JUDGE_Card_Red_Rune     = 4,
    JUDGE_Card_Blue_Rune    = 5,
} RM_JUDGE_Card_Type;

/******************************************************************************/
/* Result Struct */
typedef struct {
    uint8_t winner; // RM_JUDGE_Winner
} RM_JUDGE_Result_t; // 0x0006, 1 byte

typedef enum {
    JUDGE_Winner_Draw       = 0,
    JUDGE_Winner_Red        = 1,
    JUDGE_Winner_Blue       = 2,
} RM_JUDGE_Winner;

/******************************************************************************/
/* Buff Struct */
typedef struct {
    uint8_t buffType; // RM_JUDGE_Buff_Type
    uint8_t buffAddition; // Percentage
} RM_JUDGE_Buff_t; // 0x0007, 2 byte

typedef enum {
    JUDGE_Buff_Attack       = 0,
    JUDGE_Buff_Defense      = 1,
    JUDGE_Buff_Rune         = 2,
} RM_JUDGE_Buff_Type;

/******************************************************************************/
/* Custom Data Struct */
typedef struct {
    float data1;
    float data2;
    float data3;
    uint8_t data4;
} RM_JUDGE_Custom_Data_t; // 0x0100, 13 byte

/******************************************************************************/
/* TODO CRC8 & CRC16 Check */

#endif
