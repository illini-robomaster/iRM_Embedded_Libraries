/******************************************************************************
	Description
		Protocal Related Processing
    Log
		03/05/18 Alvin Sun      Testing Simple Protocl for VR Shooting
        03/21/18 Jeff Ma        New Protocol for communicating with TX2 board in auto aiming mode

*******************************************************************************/

#ifndef __PROTOCOL
#define __PROTOCOL

#include "mytype.h"
#include "taskProtocol.h"
#include "rm_printf.h"

#define PROTO_DATA_LENGTH         100  // 2* (sizeof(body_msg)+PROTOCOL_REQUEST_HEADER_LENGTH)
#define PROTO_BUFF_LENGTH         (sizeof(PROTOCOL_HEADER)*5 + sizeof(body_msg)*5)
#define REQUEST_RAISE             1
#define REQUEST_AWK               0
#define REQUEST_SUCC              1
#define REQUEST_FAIL              0
#define REQUEST_TOTAL_LENGTH      11
/* yaw data buffer */
extern uint16_t Yaw_data;
/* pitch data buffer */
extern uint16_t Pitch_data;
extern uint8_t protoReceiveData[PROTO_DATA_LENGTH];


uint8_t get_CRC8(uint8_t *data, uint16_t length, uint8_t crc8);

bool check_CRC8(uint8_t *data, uint16_t length);

#ifdef _CLION_FLAG
typedef struct{
#else
typedef __packed struct{
#endif
    uint8_t     CODE[4];
    uint16_t    body_length;
    uint8_t     header_CRC;
#ifdef _CLION_FLAG
} __packed PROTOCOL_HEADER;
#else
} PROTOCOL_HEADER;
#endif

#ifdef _CLION_FLAG
typedef struct{
#else
typedef __packed struct{
#endif
    uint16_t     COMMAND;
    uint8_t      data;
    uint8_t      body_CRC;
#ifdef _CLION_FLAG
} __packed PROTOCOL_REQUEST;
#else
} PROTOCOL_REQUEST;
#endif

#ifdef _CLION_FLAG
typedef struct{
#else
typedef __packed struct{
#endif
    uint16_t     COMMAND;
    uint8_t      body_CRC;
#ifdef _CLION_FLAG
} __packed PROTOCOL_ACK;
#else
} PROTOCOL_ACK;
#endif

#ifdef _CLION_FLAG
typedef struct{
#else
typedef __packed struct{
#endif
    uint16_t COMMAND_ID;
    uint16_t pitch_ref;
    uint16_t yaw_ref;
    uint8_t  data_CRC;
#ifdef _CLION_FLAG
} __packed PROTOCOL_RECEIVE_GIMBAL;
#else
} PROTOCOL_RECEIVE_GIMBAL;
#endif

typedef union {
    PROTOCOL_REQUEST body_request;
    PROTOCOL_RECEIVE_GIMBAL body_receive;
} body_msg;



/* Global protocol data buffer declaration */
extern uint8_t protoRxData[PROTO_DATA_LENGTH];

/* functions called by gimbal task */
/* function called from gimbal.c to raise a protocol request, return on PROC_SUCC or PROC_FAIL*/
uint8_t RaiseRequest(void);


/* helper functions call in protocol thread */
/* protocol initialize */
void protoInit(void);
/* protocol data read, return on PROC_SUCC or PROC_FAIL
 * if return on success, data_package contains angle info
 *  |_________|_________|
 * 32 YawData    Pitch  0
 **/
uint8_t protoDataProc(void);
/* protocol request sending, no return value */
uint8_t protoRequest(void);

#endif
