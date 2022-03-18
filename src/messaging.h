/*
	Copyright (c) 2022 by Martin Maier (martin@mcmaier.net)
	
    Permission is hereby granted, free of charge, to any person 
    obtaining a copy of this software and associated documentation 
    files (the "Software"), to deal in the Software without 
    restriction, including without limitation the rights to use, copy, 
    modify, merge, publish, distribute, sublicense, and/or sell copies 
    of the Software, and to permit persons to whom the Software is 
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be 
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.    
*/

#ifndef MESSAGING_H
#define MESSAGING_H

#include <inttypes.h>
#include <stdbool.h>

//******************************************************************************
// CONDITIONAL COMPILATION
//******************************************************************************

#ifdef MESSAGING_C
#define XTRN_MESSAGING
#else
#define XTRN_MESSAGING extern
#endif

//******************************************************************************
// DEFINITIONS
//******************************************************************************

#define MAX_MSG_BUF_LENGTH 32

typedef struct
{
    uint8_t msg_packet_id;
    uint8_t msg_data_length;      
} Message_type_t;

typedef struct 
{    
    Message_type_t dtu_ping;
    Message_type_t dtu_datetime;
    Message_type_t inverter_01;
    Message_type_t inverter_02;
    Message_type_t inverter_03;
    Message_type_t inverter_04;
    Message_type_t inverter_83;
    Message_type_t inverter_85;
    Message_type_t invalid;

} Message_set_t;

typedef struct 
{
    uint8_t command;
    uint8_t rx_address[4];
    uint8_t tx_address[4];
    Message_type_t message_type;  
    uint8_t message_buffer[MAX_MSG_BUF_LENGTH];
    uint8_t crc8;
    
} Message_t;


//Minimum number of bytes in a message (Command, Addresses, CRC8)
#define BASIC_PACKET_LENGTH         11

#define DTU_PING__PACKET_ID 		0x81
#define DTU_PING__DATA_LENGTH 		0		

#define DTU_DATETIME__PACKET_ID 	0x80
#define DTU_DATETIME__DATA_LENGTH 	16		

#define INV_01__PACKET_ID 			0x01
#define INV_01__DATA_LENGTH 		16		

#define INV_02__PACKET_ID 			0x02
#define INV_02__DATA_LENGTH 		16		

#define INV_03__PACKET_ID 			0x03
#define INV_03__DATA_LENGTH 		16		

#define INV_04__PACKET_ID 			0x04
#define INV_04__DATA_LENGTH 		16		

#define INV_83__PACKET_ID 			0x83
#define INV_83__DATA_LENGTH 		12	

#define INV_85__PACKET_ID 			0x85
#define INV_85__DATA_LENGTH 		12		

#define INVALID__PACKET_ID 			0xFF
#define INVALID__DATA_LENGTH 		0	


//******************************************************************************
// VARIABLES
//******************************************************************************

extern uint32_t sys_timer;

XTRN_MESSAGING uint8_t dtu_address[5];
XTRN_MESSAGING uint8_t inv_address[5];

XTRN_MESSAGING Message_t tx_message;
XTRN_MESSAGING Message_t rx_message;

//******************************************************************************
// FUNCTION DECLARATIONS
//******************************************************************************

XTRN_MESSAGING void messaging_set_dtu_address(uint8_t*);

XTRN_MESSAGING void messaging_set_inv_address(uint8_t*);

XTRN_MESSAGING void messaging_prepare_tx_message(Message_set_t* msg);

XTRN_MESSAGING uint8_t message_builder(Message_t* msg);

/*
XTRN_CALC void sendDataStruct(uint8_t, uint8_t*);

XTRN_CALC void readReceiveBuffer(void);

XTRN_CALC void pollSerialBuffer(void);

XTRN_CALC void parseData(uint8_t*, uint8_t*);
*/

#endif /* MESSAGING_H */