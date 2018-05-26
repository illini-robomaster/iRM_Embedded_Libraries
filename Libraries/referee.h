/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-04-18
 * @file    referee.h
 * @brief   Referee system utility
 * @log     2018-04-18 nickelliang
 */

#ifndef _REFEREE_H_
#define _REFEREE_H_

#include "bsp_error_handler.h"
#include "bsp_uart.h"
#include "bsp_config.h"
#include "data_process.h"

/*
 * Document version 2018/04/13 v1.4
 * Server version   2018/05/04
 * Client version   2018/05/04
 * Firmware version 
 */

#define REFEREE_SOF         0xA5
#define REFEREE_PORT        BSP_REFEREE_PORT
#define REFEREE_FIFO_SIZE   BSP_REFEREE_MAX_LEN
#define REFEREE_BUFF_SIZE   BSP_REFEREE_MAX_LEN

typedef enum {
    CMD_GAME_ROBOT_INFO     = 0x0001,
    CMD_DAMAGE_DATA         = 0x0002,
    CMD_SHOOT_DATA          = 0x0003,
    CMD_POWER_HEAT_DATA     = 0x0004,
    CMD_RFID_DATA           = 0x0005,
    CMD_GAME_RESULT         = 0x0006,
    CMD_BUFF_DATA           = 0x0007,
    CMD_ROBOT_POSITION      = 0x0008,
    CMD_CUSTOM_DATA         = 0x0100,
} referee_cmdid_t;

/* ===== CMD_GAME_ROBOT_INFO 0x0001 ===== */
typedef struct {
    uint16_t    stage_remain_time;  // Remaining time in the current round (seconds)
    uint8_t     game_process;       // Current stage [game_process_t]
    uint8_t     robot_grade;        // Robot's current grade
    uint16_t    remain_hp;          // Robot's current HP
    uint16_t    max_hp;             // Robot's maximum HP
} __packed game_robot_info_t;

typedef enum {
    GAME_NOT_START      = 0,        // Pre-competition stage
    GAME_PREP           = 1,        // Preparation stage
    GAME_INIT           = 2,        // Initialization stage
    GAME_5S_CNT         = 3,        // 5-second countdown
    GAME_IN_GAME        = 4,        // In combat
    GAME_RESULT         = 5,        // Calculating competition result
} game_process_t;

/* ===== CMD_DAMAGE_DATA 0x0002 ===== */
typedef struct {
    uint8_t armor_damage:4;         // Indicate armor ID if damage type is armor damage [armor_damage_t]
    uint8_t damage_type:4;          // Type of damage [damage_type_t]
} __packed damage_data_t;

typedef enum {
    ARMOR_DAMAGE_FRONT  = 0,        // Front armor damaged
    ARMOR_DAMAGE_LEFT   = 1,        // Left armor damaged
    ARMOR_DAMAGE_REAR   = 2,        // Rear armor damaged
    ARMOR_DAMAGE_RIGHT  = 3,        // Right armor damaged
    ARMOR_DAMAGE_TOP_1  = 4,        // Top armor 1 damaged
    ARMOR_DAMAGE_TOP_2  = 5,        // TOP armor 2 damaged
} armor_damage_t;

typedef enum {
    DAMAGE_ARMOR        = 0,        // Armor damaged
    DAMAGE_MOD_OFFLINE  = 1,        // Module offline damage
    // DAMAGE_SPEED_LIMIT  = 2,        // Projectile exceeds launching speed limit
    // DAMAGE_RATE_LIMIT   = 3,        // Projectile exceeds launching rate limit
    // DAMAGE_OVERHEAT     = 4,        // Barrel overheat
    // DAMAGE_POWER_LIMIT  = 5,        // Chassis power over run
} damage_type_t;

/* ===== CMD_SHOOT_DATA 0x0003 ===== */
typedef struct {
    uint8_t bullet_type;            // Projectile type [bullet_type_t]
    uint8_t bullet_freq;            // Projectile launching frequency (bullets per second)
    float   bullet_spd;             // Projectile launching speed (meters per second)
} __packed shoot_data_t;

typedef enum {
    BULLET_17MM         = 1,        // 17mm projectile
    BULLET_42MM         = 2,        // 42mm projectile
} bullet_type_t;

/* ===== CMD_POWER_HEAT_DATA 0x0004 ===== */
typedef struct {
    float       chassis_volt;       // Chassis output voltage (volt)
    float       chassis_current;    // Chassis output current (ampere)
    float       chassis_power;      // Chassis output power (watt)
    float       chassis_pwr_buf;    // Chassis power buffer (watt)
    uint16_t    barrel_heat_17;     // 17mm barrel heat
    uint16_t    barrel_heat_42;     // 42mm barrel heat
} __packed power_heat_data_t;

/* ===== CMD_RFID_DATA 0x0005 ===== */
typedef struct {
    uint8_t card_type;              // Card type [card_type_t]
    uint8_t card_idx;               // Card index number; used to distinguish different sections
} __packed rfid_data_t;

typedef enum {
    CARD_ATTACK_BUFF    = 0,        // Attack buff card
    CARD_DEFENSE_BUFF   = 1,        // Defense buff card
    CARD_RED_HEAL       = 2,        // Red team heal card
    CARD_BLUE_HEAL      = 3,        // Blue team heal card
    CARD_RED_CURE       = 4,        // Red team cure card
    CARD_BLUE_CURE      = 5,        // Blue team cure card
    CARD_RED_COOL_DOWN  = 6,        // Red team cool down card
    CARD_BLUE_COOL_DOWN = 7,        // Blue team cool down card
    CARD_FORT           = 8,        // Fort card
    CARD_RESERVE        = 9,        // Reserved card
    CARD_RESOURCE       = 10,       // Resource island card
    CARD_ICRA           = 11,       // ICRA large rune hit point card
} card_type_t;

/* ===== CMD_GAME_RESULT 0x0006 ===== */
typedef struct {
    uint8_t result;                 // Competition result [result_t]
} __packed game_result_t;

typedef enum {
    RESULT_DRAW         = 0,        // Draw
    RESULT_RED          = 1,        // Red team win
    RESULT_BLUE         = 2,        // Blue team win
} result_t;

/* ===== CMD_BUFF_DATA 0x0007 ===== */
typedef struct {
    uint16_t buff_heal:1;               // 00 Heal by heal point
    uint16_t buff_engineer:1;           // 01 Heal by engineer robot
    uint16_t buff_cure:1;               // 02 Heal by cure card
    uint16_t buff_res_defense:1;        // 03 Defense buff by resource island
    uint16_t buff_l_rune_friendly:1;    // 04 Our team activated large rune
    uint16_t buff_l_rune_enemy:1;       // 05 Enemy team activated large rune
    uint16_t buff_s_rune_friendly:1;    // 06 Our team activated small rune
    uint16_t buff_s_rune_enemy:1;       // 07 Enemy team activated small rune
    uint16_t buff_cool_down:1;          // 08 Cool down accelerated
    uint16_t buff_fort_defense:1;       // 09 Defense buff by fort
    uint16_t buff_full_defense:1;       // 10 100% Defense
    uint16_t buff_base_defense_off:1;   // 11 Base defense without sentry
    uint16_t buff_base_defense_on:1;    // 12 Base defense with sentry
    uint16_t buff_reserve:3;            // 13:15 Reserved
} __packed buff_data_t;

/* ===== CMD_ROBOT_POSITION 0x0008 ===== */
typedef struct {
    float position_x;               // Position X (meter)
    float position_y;               // Position Y (meter)
    float position_z;               // Position Z (meter)
    float barrel_yaw;               // Barrel Yaw (degree)
} __packed robot_position_t;

/* ===== CMD_CUSTOM_DATA 0x0100 ===== */
typedef struct {
    float   data1;                  // Custom data 1
    float   data2;                  // Custom data 2
    float   data3;                  // Custom data 3
    uint8_t data4;                  // Custom data 4
} __packed custom_data_t;

/* ============================== */

typedef struct {
    game_robot_info_t   game_robot_info;    // 0x0001
    damage_data_t       damage_data;        // 0x0002
    shoot_data_t        shoot_data;         // 0x0003
    power_heat_data_t   power_heat_data;    // 0x0004
    rfid_data_t         rfid_data;          // 0x0005
    game_result_t       game_result;        // 0x0006
    buff_data_t         buff_data;          // 0x0007
    robot_position_t    robot_position;     // 0x0008
    custom_data_t       custom_data;        // 0x0100
} referee_t;

/**
 * Initialize referee system dma
 *
 * @param  source   A valid data process instance
 * @return 1 for success, 0 for error
 * @author Nickel_Liang
 * @date   2018-04-19
 * @note   Since referee system tx and rx will take a relatively long period of time, consider put init / pack / unpack into a thread.
 */
uint8_t referee_init(data_process_t* source);

/**
 * Referee data dispatcher. Used by data process lib.
 *
 * @param  referee    A valid referee structure
 * @param  source     A valid data process instance
 * @return            1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-04-21
 */
uint8_t referee_dispatcher(void* target_struct, data_process_t* process_struct);

/**
 * Referee data packer. Used by data process lib.
 *
 * @param  target_struct  A valid referee structure
 * @param  process_struct A valid data process instance
 * @param  cmdid          Command ID of the msg
 * @return                1 for success, 0 for failed
 * @author Nickel_Liang
 * @date   2018-05-26
 */
uint8_t referee_packer(void *target_struct, data_process_t *process_struct, uint16_t cmdid);

/**
 * Wrapper function for referee rx callback
 *
 * @author Nickel_Liang
 * @date   2018-05-23
 */
void referee_incomming(void);

/**
 * Referee forward signal set wrapper.
 *
 * @author Nickel_Liang
 * @date   2018-05-26
 */
void referee_outgoing(void);

#endif
