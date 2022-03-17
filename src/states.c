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
 
#include <util/delay.h>
#include <avr/sleep.h>

#include "statemachine.h"
//#include "ringbuffer.h"
//#include "hardware.h"
//#include "calculation.h"
//#include "uart.h"
//#include "spi.h"
//#include "nRF24L01.h"
//#include "wl_module.h"

extern volatile uint8_t PTX;
static uint16_t timer;

//static uint8_t payload[16];

//----------------------------------------------------------
// ERROR STATE HANDLER
//---------------------------------------------------------

States_t STATE_error__handler(Events_t event)
{
	States_t ret_state = STATE_error;

	switch(event)
	{	
		case EVENT_timer_tick:							
			ret_state = STATE_error;					
			break;
			
		default:
			break;
	}
	
	return ret_state;
}	


//----------------------------------------------------------
// START STATE HANDLER
//---------------------------------------------------------

States_t STATE_start__handler(Events_t event)
{
	States_t ret_state = STATE_start;

	switch(event)
	{	
		case EVENT_timer_tick:							
				ret_state = STATE_start;					
			break;
			
		default:
			break;
	}
	
	return ret_state;
}	


//----------------------------------------------------------
// INIT STATE HANDLER
//---------------------------------------------------------

States_t STATE_init__handler(Events_t event)
{
	States_t ret_state = STATE_init;

	switch(event)
	{	
		case EVENT_timer_tick:							
			ret_state = STATE_init;					
			break;
			
		default:
			break;
	}
	
	return ret_state;
}


//----------------------------------------------------------
// RECEIVE STATE HANDLER
//---------------------------------------------------------

States_t STATE_receive__handler(Events_t event)
{
	States_t ret_state = STATE_receive;
	
	/*
	switch(event)
	{	
		case EVENT_timer_tick:				
			if(timer++ == 0)
			{
				//LED1_ON;			
				configureSpi(SPI_MODE_NRF);
			}
			else if(timer == 5)
			{				
				wl_module_tx_config(wl_module_TX_NR_0);	
				wl_module_rx_config();	
			}
			else if(timer > 55)
			{				
				if(measureMode || writeMode)
				{
					wl_module_CE_lo;
					ret_state = STATE_batt_measure;
				}
				else
				{
					configureSpi(SPI_MODE_OFF);
					ret_state = STATE_batt_sleep;				
				}				
			}
			break;
			
		case EVENT_dataReceivedNRF:		
				if(wirelessRxBuf[0] == 'M')
				{
					measureMode = 1;
				}		
				else if(wirelessRxBuf[0] == 'm')
				{
					measureMode = 0;
				}
				else if(wirelessRxBuf[0] == 'W')
				{
					writeMode = 1;
				}
				else if(wirelessRxBuf[0] == 'w')
				{
					writeMode = 0;
				}
			break;
			
		default:
			break;
	}
	*/
	
	return ret_state;
}	

//----------------------------------------------------------
// TRANSMIT STATE HANDLER
//---------------------------------------------------------

States_t STATE_transmit__handler(Events_t event)
{
	States_t ret_state = STATE_transmit;

	/*

	switch(event)
	{	
		case EVENT_timer_tick:	
				
			if(timer++ == 0)
			{		
				LED1_ON
								
				//configureSpi(SPI_MODE_NRF);
	
				i2c_start(RTC_ADDRESS|I2C_WRITE);
				i2c_write(0x01);
				i2c_rep_start(RTC_ADDRESS|I2C_READ);
				payload[0] = i2c_readAck();
				payload[1] = i2c_readAck();
				payload[2] = i2c_readNak();
			}
			else if(timer == 2)
			{
				ADCSRA |= (1 << ADEN);
				LIGHT_MEAS_ON;
				ADC_Read(7);				
			}
			else if(timer == 3)
			{
				adc = ADC_Read(7);
				LIGHT_MEAS_OFF;
				
				payload[3] = (uint8_t)(adc & 0x00FF);
				payload[4] = (uint8_t)((adc & 0xFF00) >> 8);
				
				adc = ADC_Read(6);
			}
			else if(timer == 4)
			{
				adc = ADC_Read(6);
				ADCSRA &= ~(1 << ADEN);
				
				payload[5] = (uint8_t)(adc & 0x00FF);
				payload[6] = (uint8_t)((adc & 0xFF00) >> 8);
			}
			else if(timer == 5)
			{			
				//wl_module_rx_config();
				//wl_module_tx_config(wl_module_TX_NR_0);	
				
				if(measureMode)
				{
					wl_module_send(payload,wl_module_PAYLOAD);
				}
				
				if(writeMode)
				{
					parseData(payload,writeBuf[writeCount]);
				}
			}			
			else if((timer > 5) && !PTX)
			{			
				LED1_OFF
		
				if(writeMode)
				{
					if(writeCount++ > 4)
					{
						ret_state = STATE_batt_write;
						writeCount = 0;
					}
					else
					{
						ret_state = STATE_batt_sleep;
					}
				}
				else
				{
					ret_state = STATE_batt_sleep;
				}
			}
			else if(PTX && (timer > 50))
			{
				PTX = 0;
			}
			break;
			
		default:
			break;
	}
	
	if(ret_state == STATE_batt_sleep)
	{
		configureSpi(SPI_MODE_OFF);
	}

	*/
	
	return ret_state;
}	


//----------------------------------------------------------
// STATE ENTRY FUNCTIONS
//---------------------------------------------------------
void STATE_error__entering_handler(void)
{	
	timer = 0;
}


//---------------------------------------------------------
void STATE_start__entering_handler(void)
{	
	timer = 0;
}

//---------------------------------------------------------
void STATE_init__entering_handler(void)
{	
	timer = 0;
}

//---------------------------------------------------------
void STATE_receive__entering_handler(void)
{	
	timer = 0;
}


//---------------------------------------------------------
void STATE_transmit__entering_handler(void)
{
	timer = 0;
}
