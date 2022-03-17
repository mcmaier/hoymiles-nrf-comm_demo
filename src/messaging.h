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
    Message_set_t message_type;  
    uint8_t message_buffer[MAX_MSG_BUF_LENGTH];
    uint8_t crc8;
    
} Message_t;


//******************************************************************************
// FUNCTION DECLARATIONS
//******************************************************************************

/*
XTRN_CALC void sendDataStruct(uint8_t, uint8_t*);

XTRN_CALC void readReceiveBuffer(void);

XTRN_CALC void pollSerialBuffer(void);

XTRN_CALC void parseData(uint8_t*, uint8_t*);
*/

#endif /* MESSAGING_H */