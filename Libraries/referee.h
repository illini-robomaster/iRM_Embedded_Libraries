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

#define REFEREE_SOF 0xA5

typedef enum {
    CMD_GAME_ROBOT_INFO     = 0x0001,
    CMD_DAMAGE_DATA         = 0x0002,
    CMD_SHOOT_DATA          = 0x0003,
    CMD_POWER_HEAT_DATA     = 0x0004,
    CMD_RFID_DATA           = 0x0005,
    CMD_GAME_RESULT         = 0x0006,
    CMD_BUFF_DATA           = 0x0007,
    CMD_CUSTOM_DATA         = 0x0100,
} referee_cmdid_t;

/* ===== CMD_GAME_ROBOT_INFO 0x0001 ===== */
typedef struct {
    uint16_t    stage_remain_time;  // Remaining time in the current round (seconds)
    uint8_t     game_process;       // Current stage [game_process_t]
    uint8_t     robot_grade;        // Robot's current grade
    uint16_t    remain_hp;          // Robot's current HP
    uint16_t    max_hp;             // Robot's maximum HP
    uint8_t     location_valid_flag;// Location/angle information effective zone bit [location_valid_flag_t]
} __packed game_robot_info_t;

typedef enum {
    GAME_NOT_START      = 0,        // Pre-competition stage
    GAME_PREP           = 1,        // Preparation stage
    GAME_INIT           = 2,        // Initialization stage
    GAME_5S_CNT         = 3,        // 5-second countdown
    GAME_IN_GAME        = 4,        // In combat
    GAME_RESULT         = 5,        // Calculating competition result
} game_process_t;

typedef enum {
    LOCATION_INVALID    = 0,        // Location invalid
    LOCATION_VALID      = 1,        // Location valid
} location_valid_flag_t;

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
} armor_damage_t;

typedef enum {
    DAMAGE_ARMOR        = 0,        // Armor damaged
    DAMAGE_MOD_OFFLINE  = 1,        // Module offline damage
    DAMAGE_SPEED_LIMIT  = 2,        // Projectile exceeds launching speed limit
    DAMAGE_RATE_LIMIT   = 3,        // Projectile exceeds launching rate limit
    DAMAGE_OVERHEAT     = 4,        // Barrel overheat
    DAMAGE_POWER_LIMIT  = 5,        // Chassis power over run
} damage_type_t;

/* ===== CMD_SHOOT_DATA 0x0003 ===== */
typedef struct {
    uint8_t bullet_type;            // Projectile type [bullet_type_t]
    uint8_t bullet_freq;            // Projectile launching frequency
    float   bullet_spd;             // Projectile launching speed
    float   reserved;               // Reserved
} __packed shoot_data_t;

typedef enum {
    BULLET_17MM         = 1,        // 17mm projectile
    BULLET_42MM         = 2,        // 42mm projectile
} bullet_type_t;

/* ===== CMD_POWER_HEAT_DATA 0x0004 ===== */
typedef struct {
    float       chassis_volt;       // Chassis output voltage
    float       chassis_current;    // Chassis output current
    float       chassis_power;      // Chassis output power
    float       chassis_pwr_buf;    // Chassis power buffer
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
    CARD_RED_RUNE       = 4,        // Red team rune card
    CARD_BLUE_RUNE      = 5,        // Blue team rune card
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
    uint8_t buff_type;              // Buff categories [buff_type_t]
    uint8_t buff_percent;           // Buff percentage (10 represents 10% buff)
} __packed buff_data_t;

typedef enum {
    BUFF_ATTACK         = 0,        // Attack buff
    BUFF_DEFENSE        = 1,        // Defense buff
    BUFF_RUNE           = 2,        // Rune buff (high power)
} buff_type_t;

/* ===== CMD_CUSTOM_DATA 0x0100 ===== */
typedef struct {
    float   data1;                  // Custom data 1
    float   data2;                  // Custom data 2
    float   data3;                  // Custom data 3
    uint8_t data4;                  // Custom data 4
} __packed custom_data_t;

/* ============================== */

typedef struct {
    game_robot_info_t   game_robot_info;    //0x0001
    damage_data_t       damage_data;        //0x0002
    shoot_data_t        shoot_data;         //0x0003
    power_heat_data_t   power_heat_data;    //0x0004
    rfid_data_t         rfid_data;          //0x0005
    game_result_t       game_result;        //0x0006
    buff_data_t         buff_data;          //0x0007
    custom_data_t       custom_data;        //0x0100
} referee_t;

/**
 * Initialize referee system dma
 *
 * @return 1 for success, 0 for error
 * @author Nickel_Liang
 * @date   2018-04-19
 * @note   Since referee system tx and rx will take a relatively long period of time, consider put init / pack / unpack into a thread.
 */
uint8_t referee_init(void);


#endif
