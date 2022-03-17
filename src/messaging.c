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

#ifndef MESSAGING_C
#define MESSAGING_C

#include "messaging.h"
#include "crc.h"

#include <stdlib.h>
#include <string.h>

//#include "hardware.h"
//#include "ringbuffer.h"
//#include "crc.h"
//#include "uart.h"

//******************************************************************************
// DEFINITIONS
//******************************************************************************

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

Message_set_t message_set =
{
	.dtu_ping = {.msg_packet_id = DTU_PING__PACKET_ID, .msg_data_length = DTU_PING__DATA_LENGTH},
	.dtu_datetime = {.msg_packet_id = DTU_DATETIME__PACKET_ID, .msg_data_length = DTU_DATETIME__DATA_LENGTH},
	.inverter_01 = {.msg_packet_id = INV_01__PACKET_ID, .msg_data_length = INV_01__DATA_LENGTH},
	.inverter_02 = {.msg_packet_id = INV_02__PACKET_ID, .msg_data_length = INV_02__DATA_LENGTH},
	.inverter_03 = {.msg_packet_id = INV_03__PACKET_ID, .msg_data_length = INV_03__DATA_LENGTH},
	.inverter_04 = {.msg_packet_id = INV_04__PACKET_ID, .msg_data_length = INV_04__DATA_LENGTH},
	.inverter_83 = {.msg_packet_id = INV_83__PACKET_ID, .msg_data_length = INV_83__DATA_LENGTH},
	.inverter_85 = {.msg_packet_id = INV_85__PACKET_ID, .msg_data_length = INV_85__DATA_LENGTH},
	.invalid = {.msg_packet_id = INVALID__PACKET_ID, .msg_data_length = INVALID__DATA_LENGTH}
};

//******************************************************************************
// FUNCTIONS 
//******************************************************************************

void messaging_set_dtu_address(uint8_t* input)
{
	memcpy(dtu_address, input, 4);	
}

void messaging_set_inv_address(uint8_t* input)
{
	memcpy(inv_address, input, 4);	
}

//******************************************************************************
// PARSER 
//******************************************************************************

// parses the message according to its type
//------------------------------------------------------------------------------
void message_parser(uint8_t* message)
{
	uint8_t message_id = 0;

	//static uint8_t txBuf[I2C_MAX_MESSGAGE_LENGTH];
	//memset(txBuf, 0, I2C_MAX_MESSGAGE_LENGTH);

	switch(message_id)  
	{
	case INV_01__PACKET_ID:  
		//i2c_message__keep_alive(message,txBuf);    
		break;
		
	case INV_02__PACKET_ID: 
	// i2c_message__read_slave_message_length(message,txBuf);
		break;

	case INV_03__PACKET_ID: 
	// i2c_message__read_slave_message_length(message,txBuf);
		break;

		
	case INV_04__PACKET_ID: 
	// i2c_message__read_slave_message_length(message,txBuf);
		break;

		
	case INV_83__PACKET_ID: 
	// i2c_message__read_slave_message_length(message,txBuf);
		break;

		
	case INV_85__PACKET_ID: 
	// i2c_message__read_slave_message_length(message,txBuf);
		break;

	default:
		break;
	}
}


// parses the message according to its type
//------------------------------------------------------------------------------
uint8_t message_builder(Message_t* message, uint8_t id)
{
	if(id == DTU_PING__PACKET_ID)
	{
		message->command = 0x15;
		//message->rx_address = 0;
		//message->tx_address = 0;
		//message->crc8 = crc8_calc());

		return DTU_PING__PACKET_ID;
	}
	else if(id == DTU_DATETIME__PACKET_ID)
	{
		message->command = 0x15;

		return DTU_DATETIME__PACKET_ID;
	}
	else
	{
		return 0;
	}

}
    

//******************************************************************************
// MESSAGE HANDLERS
//******************************************************************************

static void message_parse__invalid(uint8_t* data)
{
	
}

/*

//Liest den Empfangspuffer der RS232 und verarbeitet die Information entsprechend
//----------------------
void readReceiveBuffer(void)
{	
	uint16_t crcTemp, crcCompare;

	for(int i = 0; i < 16; ++i)
	{
		receiveBuffer[i] = uart_getc();
	}
	uart_flush();
	
	crcTemp = crcFast(receiveBuffer, 14);
	crcCompare = ((((uint16_t)receiveBuffer[15]) << 8) | (uint16_t)receiveBuffer[14]);
		
	if((crcTemp == crcCompare))
	{	
		//Befehl auswerten
	}
}

//Sendet die ausgew�hlte Datenstruktur �ber die serielle Schnittstelle
//---------------------
void sendDataStruct(uint8_t indx, uint8_t* addr)
{
	static uint8_t buf[16];
	static uint8_t i;
	static uint16_t crcTemp;	
	
	memcpy(buf,addr,16);

	crcTemp = crcFast(buf, 14);
	buf[15] = (uint8_t)((crcTemp & 0xFF00) >> 8);
	buf[14] = (uint8_t)(crcTemp & 0x00FF);
	
	for(i = 0; i < 16; ++i)
	{
		uart_putc(buf[i]);
	}
}

//Sendet die ausgew�hlte Datenstruktur �ber die serielle Schnittstelle
//---------------------
void pollSerialBuffer(void)
{
	if(uart_available() >= 15)
	{
		readReceiveBuffer();
	}
}

void parseData(uint8_t* inBuffer, uint8_t* outBuffer)
{	
	char itoabuffer[10];
	uint8_t tempSec = 0;
	uint8_t tempMin = 0;
	uint8_t tempHour = 0;
	uint8_t tempDay = 0;
	uint8_t tempMon = 0;
	uint8_t tempYear = 0;
	uint16_t tempLight = 0;
	uint16_t tempBatt = 0;
	uint16_t tempTemp = 0;
	int16_t tempTempSigned = 0;
	uint16_t tempHumi = 0;
	uint16_t tempPressure = 0;
	
	memset(itoabuffer,0,10);
	memset(outBuffer,0,WRITE_BUF_SIZE);
	
	tempSec = 10*((inBuffer[0] & 0x70) >> 4) + (inBuffer[0] & 0x0F);
	tempMin = 10*((inBuffer[1] & 0x70) >> 4) + (inBuffer[1] & 0x0F);
	tempHour = 10*((inBuffer[2] & 0x30) >> 4) + (inBuffer[2] & 0x0F);	
	
	tempDay = 10*((inBuffer[3] & 0x30) >> 4) + (inBuffer[3] & 0x0F);
	tempMon = 10*((inBuffer[4] & 0x10) >> 4) + (inBuffer[4] & 0x0F);
	tempYear = 10*((inBuffer[5] & 0xF0) >> 4) + (inBuffer[5] & 0x0F);	
	
	tempLight = (((uint16_t)inBuffer[7]) << 8) | inBuffer[6];	
	tempLight = (uint16_t)(tempLight * 1.72);
	tempBatt =  (((uint16_t)inBuffer[9]) << 8) | inBuffer[8];
	tempBatt = (uint16_t)(tempBatt*0.48387);
	
	tempTemp = (((uint16_t)inBuffer[10]) << 8) | (inBuffer[11] & 0xFC);
	tempTempSigned = (int16_t)(((tempTemp*0.26812744) - 4685));
	
	tempHumi = (((uint16_t)inBuffer[12]) << 8) | (inBuffer[13] & 0xFC);
	tempHumi = (uint16_t)(tempHumi*0.0190735 - 60);
	
	tempPressure = (((uint16_t)inBuffer[14]) << 8) | (inBuffer[15]);
	
	//Date
	strcat((char*)outBuffer,"20");	
	itoa(tempYear, itoabuffer, 10);	
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer,"-");	
	itoa(tempMon, itoabuffer, 10);
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer,"-");
	itoa(tempDay, itoabuffer, 10);
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer," ");
	
	//Time
	itoa(tempHour, itoabuffer, 10);
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer,":");	
	itoa(tempMin, itoabuffer, 10);
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer,":");	
	itoa(tempSec, itoabuffer, 10);		
	strcat((char*)outBuffer,itoabuffer);
	strcat((char*)outBuffer,";");	
	
	//Measurements
	utoa(tempLight, itoabuffer, 10);			
	strcat((char*)outBuffer,itoabuffer);		
	strcat((char*)outBuffer,";");
	
	//utoa(tempBatt, itoabuffer, 10);	
	memset(itoabuffer,0,10);
	valout (tempBatt, 1, 2, itoabuffer);
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer,";");
	
	itoa(tempTempSigned, itoabuffer, 10);	
	//memset(itoabuffer,0,10);
	//valout (tempTempSigned, 1, 2, itoabuffer);
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer,";");
	
	utoa(tempHumi, itoabuffer, 10);	
	//memset(itoabuffer,0,10);
	//valout (tempHumi, 1, 1, itoabuffer);
	strcat((char*)outBuffer,itoabuffer);	
	strcat((char*)outBuffer,";");
	
	utoa(tempPressure, itoabuffer, 10);	
	//memset(itoabuffer,0,10);
	//valout (tempPressure, 1, 1, itoabuffer);
	strcat((char*)outBuffer,itoabuffer);
}

*/

#endif /* CALCULATION_C */