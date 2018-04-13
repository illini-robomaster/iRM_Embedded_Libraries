/******************************************************************************
	Description
		DBUS Utility, Connect to USART1 PB7 on RM Standard Board
    Log
		12/02/17 Nickel Liang	Copy code from instruction
		12/03/17 Nickel Liang	Test & Bug fix
        12/06/17 Nickel Liang   Data structure improvements
        12/07/17 Nickel Liang   Full KEY VALUE TABLE
                                Add debug utility
        12/08/17 Nickel Liang   Fix serial error bug
        12/19/17 Nickel Liang   Remove redundant struct member
*******************************************************************************/

#ifndef __DBUS
#define __DBUS

#include "mytype.h"
#include "led.h" // Used in init check. If use error handler in the future, remove this line
#include "buzzer.h" // Used for temp bug handler

/* Define */
// Some data related to keyboard & mouse received
#define RM_M_Action      ((remoteData.mouse.x || remoteData.mouse.y || remoteData.mouse.pressL || remoteData.mouse.pressR) ? 1 : 0)
#define RM_K_Action      (remoteData.key.raw ? 1 : 0)
// Mouse move to a direction
#define RM_Move_Left     (remoteData.mouse.x < 0 ? 1 : 0)
#define RM_Move_Right    (remoteData.mouse.x > 0 ? 1 : 0)
#define RM_Move_Down     (remoteData.mouse.y > 0 ? 1 : 0)
#define RM_Move_Up       (remoteData.mouse.y < 0 ? 1 : 0)
// Mouse clicked
#define RM_Left_Clicked  (remoteData.mouse.pressL ? 1 : 0)
#define RM_Right_Clicked (remoteData.mouse.pressR ? 1 : 0)
// Key pressed
#define RM_W_Pressed     (remoteData.key.w ? 1 : 0)
#define RM_S_Pressed     (remoteData.key.s ? 1 : 0)
#define RM_A_Pressed     (remoteData.key.a ? 1 : 0)
#define RM_D_Pressed     (remoteData.key.d ? 1 : 0)
#define RM_SHIFT_Pressed (remoteData.key.shift ? 1 : 0)
#define RM_CTRL_Pressed  (remoteData.key.ctrl ? 1 : 0)
#define RM_Q_Pressed     (remoteData.key.q ? 1 : 0)
#define RM_E_Pressed     (remoteData.key.e ? 1 : 0)
#define RM_R_Pressed     (remoteData.key.r ? 1 : 0)
#define RM_F_Pressed     (remoteData.key.f ? 1 : 0)
#define RM_G_Pressed     (remoteData.key.g ? 1 : 0)
#define RM_Z_Pressed     (remoteData.key.z ? 1 : 0)
#define RM_X_Pressed     (remoteData.key.x ? 1 : 0)
#define RM_C_Pressed     (remoteData.key.c ? 1 : 0)
#define RM_V_Pressed     (remoteData.key.v ? 1 : 0)
#define RM_B_Pressed     (remoteData.key.b ? 1 : 0)

/* DBUS Data Decode */
typedef enum {
    DBUS_RAW_LEN    = 18,

    DBUS_CH4_MIN    = 300,    // Not test yet
    DBUS_CH_MIN     = 364,
    DBUS_CH_OFFSET  = 1024,
    DBUS_CH_MAX     = 1684,
    DBUS_SWL_UP     = 1,      // Not test yet
    DBUS_SWL_MID    = 1,      // Not test yet
    DBUS_SWL_DOWN   = 0,      // Not test yet
    DBUS_SWR_UP     = 1,
    DBUS_SWR_MID    = 3,
    DBUS_SWR_DOWN   = 2,

    // DBUS_MOUSE_MAX  = 32767,  // Moving Right or Down
    DBUS_MOUSE_MAX  = 8191,  // v0707 only sent 14 bits
    DBUS_MOUSE_HOLD = 0,
    // DBUS_MOUSE_MIN  = -32768, // Moving Left or Up
    DBUS_MOUSE_MIN  = -8192,

    DBUS_W          = 0x0001, // 0x01<<0
    DBUS_S          = 0x0002, // 0x01<<1
    DBUS_A          = 0x0004, // 0x01<<2
    DBUS_D          = 0x0008, // 0x01<<3
    DBUS_SHIFT      = 0x0010, // 0x01<<4
    DBUS_CTRL       = 0x0020, // 0x01<<5    Or Command / Windows key
    DBUS_Q          = 0x0040, // 0x01<<6
    DBUS_E          = 0x0080, // 0x01<<7    Currently not working v0707
    DBUS_R          = 0x0100, // 0x01<<8
    DBUS_F          = 0x0200, // 0x01<<9
    DBUS_G          = 0x0400, // 0x01<<10
    DBUS_Z          = 0x0800, // 0x01<<11
    DBUS_X          = 0x1000, // 0x01<<12
    DBUS_C          = 0x2000, // 0x01<<13
    DBUS_V          = 0x4000, // 0x01<<14
    DBUS_B          = 0x8000, // 0x01<<15   Currently not working v0707
} RM_DBUS;

/* DBUS Data Structure */
typedef struct {
    struct {
        uint16_t ch0;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;
        uint8_t s1;
        uint8_t s2;
    } rc;

    struct {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t pressL;
        uint8_t pressR;
    } mouse;

    struct {
        uint16_t w;
        uint16_t s;
        uint16_t a;
        uint16_t d;
        uint16_t shift;
        uint16_t ctrl;
        uint16_t q;
        uint16_t e;
        uint16_t r;
        uint16_t f;
        uint16_t g;
        uint16_t z;
        uint16_t x;
        uint16_t c;
        uint16_t v;
        uint16_t b;

        uint16_t raw;
    } key;
} dbus_t;

/* Declaration */
extern dbus_t remoteData;
extern uint8_t dbusRxData[DBUS_RAW_LEN];

/* Global Functions */
void RM_DBUS_DataProcess(uint8_t *pData);
void RM_DBUS_Init(void);
void RM_DBUS_InitCheck(void);

/* Debug Utility Functions */
void RM_DEBUG_DBUS_PrintRaw(void);
void RM_DEBUG_DBUS_PrintValue(void);
void RM_DEBUG_DBUS_PrintAction(void);

#endif
