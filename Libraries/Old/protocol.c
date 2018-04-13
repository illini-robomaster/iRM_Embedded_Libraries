/******************************************************************************
	Description
		STM32 TX2 protocol
    Log
		12/02/17  Jeff Ma      build basic APIs
*******************************************************************************/
#include <key.h>
#include "protocol.h"
#define PROTOCOL_REQUEST_COMMAND      0x0012
#define DAFU_DATA					  0x0001
#define PROTOCOL_REQUEST_ACK		  0x0000
#define PROTOCOL_RECEIVE_COMMMAND	  0x00A1
uint8_t protoReceiveData[PROTO_DATA_LENGTH];

static const uint8_t k_crc8 = 0xff;
static const uint8_t crc8_table[256] = {
		0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e,
		0x20, 0xa3, 0xfd, 0x1f, 0x41, 0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2,
		0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 0x23,
		0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03,
		0x80, 0xde, 0x3c, 0x62, 0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63,
		0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 0x46, 0x18,
		0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5,
		0xbb, 0x59, 0x07, 0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
		0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 0x65, 0x3b, 0xd9,
		0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98,
		0x7a, 0x24, 0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a,
		0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 0x8c, 0xd2, 0x30, 0x6e,
		0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93,
		0xcd, 0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d,
		0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 0xaf, 0xf1, 0x13, 0x4d, 0xce,
		0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
		0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c,
		0x12, 0x91, 0xcf, 0x2d, 0x73, 0xca, 0x94, 0x76, 0x28, 0xab, 0xf5,
		0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 0x57,
		0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77,
		0xf4, 0xaa, 0x48, 0x16, 0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34,
		0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 0x74, 0x2a,
		0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7,
		0x89, 0x6b, 0x35
};


/* raise request flag */
static uint8_t raiseReq;
/* yaw data buffer */
uint16_t Yaw_data;
/* pitch data buffer */
uint16_t Pitch_data;

uint8_t get_CRC8(uint8_t *data, uint16_t length, uint8_t crc8){
	uint8_t index;
	while(length--){
		index = crc8 ^ *(data++);
		crc8 = crc8_table[index];
	}
	return crc8; 
}

bool check_CRC8(uint8_t *data, uint16_t length ){
	if( length <= 2 || data == NULL)
		return false;
	return (get_CRC8(data, length-1, k_crc8) == data[length-1]);
}

/*
 * 4b:"IRM"
 * 2b: length
 * 1b: header CRC
 * 2b: command : oo12
 * 1b: data 0: da'fu 1:assist aiming
 * 1b: boby scr*/

uint8_t RaiseRequest(void){
    if (raiseReq==REQUEST_AWK){
        raiseReq = REQUEST_RAISE; 
        return REQUEST_SUCC; 
    }
    else 
        return REQUEST_FAIL;
}

void protoInit(void){
    raiseReq = REQUEST_AWK;
    Yaw_data = Pitch_data = 0;
	if (HAL_UART_Receive_IT(&huart3, protoReceiveData, PROTO_BUFF_LENGTH)!=HAL_OK)
		return ;
    return ;
}
#define PROTOCOL_REQUEST_HEADER_LENGTH		7

uint8_t protoRequest(void){
    /* initial new request */ 
    PROTOCOL_HEADER new_req_header = {"IRM", sizeof(PROTOCOL_REQUEST), 0};
    PROTOCOL_REQUEST new_req_body = {PROTOCOL_REQUEST_COMMAND, DAFU_DATA, 0};
    // update CRC
    new_req_header.header_CRC = get_CRC8((uint8_t*) new_req_header.CODE, PROTOCOL_REQUEST_HEADER_LENGTH - 1, k_crc8);
    new_req_body.body_CRC = get_CRC8((uint8_t*) &(new_req_body), new_req_header.body_length - 1, k_crc8);
    uint8_t local_buffer[REQUEST_TOTAL_LENGTH*2];
    /* check if there is a request to send */
    if(raiseReq == REQUEST_AWK)
        return REQUEST_FAIL;
	RM_PRINTF("NEED SENDING REQ\t");
    /* if yes then prepare to send the request */
    raiseReq = REQUEST_AWK;
 	/* initizal request package */
    memcpy((void *) local_buffer, (void *) &new_req_header, sizeof(PROTOCOL_HEADER));
    memcpy((void *) (local_buffer+sizeof(PROTOCOL_HEADER)), (void *) &new_req_body, sizeof(PROTOCOL_REQUEST));

    /* if sent request but uart busy */
    if(HAL_UART_Transmit(&huart3, local_buffer, REQUEST_TOTAL_LENGTH, 500) != HAL_OK) {
        return REQUEST_FAIL;
    }

    RM_PRINTF("SENDING SUCC\r\n");
    return REQUEST_SUCC;
}

uint8_t protoACK(void){
	/* initial new request */
	PROTOCOL_HEADER new_req_header = {"IRM", sizeof(PROTOCOL_ACK), 0};
	PROTOCOL_ACK new_req_body = {PROTOCOL_REQUEST_ACK, 0};
	// update CRC
	new_req_header.header_CRC = get_CRC8((uint8_t*) new_req_header.CODE, PROTOCOL_REQUEST_HEADER_LENGTH - 1, k_crc8);
	new_req_body.body_CRC = get_CRC8((uint8_t*) &(new_req_body), new_req_header.body_length - 1, k_crc8);
	uint8_t local_buffer[REQUEST_TOTAL_LENGTH*2];
	/* initizal request package */
	memcpy((void *) local_buffer, (void *) &new_req_header, sizeof(PROTOCOL_HEADER));
	memcpy((void *) (local_buffer+sizeof(PROTOCOL_HEADER)), (void *) &new_req_body, sizeof(PROTOCOL_ACK));

	/* if sent request but uart busy */
	if(HAL_UART_Transmit(&huart3, local_buffer,  sizeof(PROTOCOL_HEADER) + sizeof(PROTOCOL_ACK), 500) != HAL_OK){
		return REQUEST_FAIL;
	}

	return REQUEST_SUCC;
}

uint8_t protoDataProc(void) {
	uint8_t receive_flag = 0;	// the flag to check if a valid data package is received
	uint8_t *data_start = protoReceiveData;	// decode the header first
    PROTOCOL_HEADER *receive_data_header;	// data structure to decode the header
	PROTOCOL_RECEIVE_GIMBAL *receive_data_body; 	// data structure to decode the body
	uint32_t i;

	/* set up to receive the data */
    RM_PRINTF("RECEIVE ATTEMPT %u\t", (uint8_t) (protoReceiveData[0]));


	/* check if the received buffer contains data header */ 
	for( i = 0; i <= PROTO_BUFF_LENGTH - PROTOCOL_REQUEST_HEADER_LENGTH; i++){ // traverse buffer to find header first
		if(check_CRC8(data_start, PROTOCOL_REQUEST_HEADER_LENGTH)){
			// found one header
			receive_flag = 1; // set flag
            receive_data_header = (PROTOCOL_HEADER *) data_start;
			// check body message whole
            if (i+PROTOCOL_REQUEST_HEADER_LENGTH+receive_data_header->body_length >= PROTO_BUFF_LENGTH)
                return REQUEST_FAIL;
			break;
		}
		// check next
		data_start ++;
	}

	if (receive_flag == 0)
		return REQUEST_FAIL;
	// check body data valid
    if(!check_CRC8(data_start+PROTOCOL_REQUEST_HEADER_LENGTH, receive_data_header->body_length)) {
        return REQUEST_FAIL;
    }

    receive_data_body = (PROTOCOL_RECEIVE_GIMBAL *) (data_start+PROTOCOL_REQUEST_HEADER_LENGTH);
	if (receive_data_body->COMMAND_ID==PROTOCOL_REQUEST_COMMAND) {
		RM_PRINTF("READ_SELF");
		Yaw_data = 11;
		Pitch_data = 12;
		SetProStatusValid();
		protoACK();
		return REQUEST_SUCC;
	}
	if (receive_data_body->COMMAND_ID!=PROTOCOL_RECEIVE_COMMMAND)
		return REQUEST_FAIL;
    Yaw_data = receive_data_body->yaw_ref;
    Pitch_data = receive_data_body->pitch_ref;
	SetProStatusValid();
	protoACK();
    return REQUEST_SUCC;
}
