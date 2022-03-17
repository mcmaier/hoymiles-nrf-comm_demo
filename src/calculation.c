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

#ifndef CALCULATION_C
#define CALCULATION_C

#include <stdlib.h>
#include <string.h>

#include "calculation.h"

//#include "hardware.h"
//#include "ringbuffer.h"
//#include "crc.h"
//#include "uart.h"

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