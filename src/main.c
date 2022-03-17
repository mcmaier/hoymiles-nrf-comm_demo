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

//---------------------------------------------------------------------
// Code for AVR & NRF24 for communication with hoymiles microinverters
//---------------------------------------------------------------------
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "statemachine.h"
#include "calculation.h"
#include "hardware.h"
#include "wl_module.h"
#include "spi.h"
#include "nRF24L01.h"
#include "uart.h"

volatile uint8_t PTX;
States_t main_state;

ISR(INT0_vect)
{
    volatile uint8_t status;   
    
	// Read wl_module status 
	wl_module_CSN_lo;                               // Pull down chip select
	status = spi_fast_shift(NOP);					// Read status register
	wl_module_CSN_hi;                               // Pull up chip select
	
	switch(status & 0x70)
	{
		case (1 << TX_DS):		
			PTX = 0;	
		break;
		
		case (1 << MAX_RT):
			wl_module_CSN_lo;                               // Pull down chip select
			spi_fast_shift(FLUSH_TX);						// Flush TX-FIFO
			wl_module_CSN_hi;  
			PTX = 0;					
		break;
		
		case (1 << RX_DR):
			//wl_module_get_data(rxBuf);
			//bufferWrite(&wirelessRxBuf, rxBuf);

		break;
		
		case ((1 << RX_DR) | (1 << TX_DS)):
			//wl_module_get_data(rxBuf);
			//bufferWrite(&wirelessRxBuf, rxBuf);
			PTX = 0;
		break;	
	}
		
	if (status & (1<<TX_FULL))							//TX_FIFO Full <-- this is not an IRQ
	{
		wl_module_CSN_lo;  
		spi_fast_shift(FLUSH_TX);
		wl_module_CSN_hi;  
		PTX = 0;
	}
		
	wl_module_config_register(STATUS, status & 0x70);	
}

ISR(SPI_STC_vect)
{
}

ISR(TIMER1_COMPA_vect)
{
	event_push(EVENT_timer_tick);
}

int main(void)
{	
	hardware_init();
	wl_module_init();
	state_machine__init();
		
	_delay_ms(50);

	sei();

	while(1)
    {
		Events_t temp_event;

		while (event_get(&temp_event))
		{
			main_state = state_machine__calculate_state(temp_event);
		}
	}
}


//Old code for info
/*
		
		if(uart_available() >= DATA_PAYLOAD)
		{		
			for(i = 0; i < DATA_PAYLOAD; ++i)
			{
				txBuf[i] = (uint8_t)(uart_getc() & 0x00FF);				
			}
			bufferWrite(&serialRxBuf,txBuf);
		}
		
		read = bufferRead(&wirelessRxBuf,txBuf);
		
		if(!read)
		{
			setLed2();		
			uart_putc_arr((const char*)txBuf,DATA_PAYLOAD);
		}
	
		if(transmitter)
		{					
			if(!PTX)
			{	
				read = bufferRead(&serialRxBuf,txBuf);
			
				if(!read)
				{
					setLed1();		
					transmitter = 1;
					wl_module_send(txBuf,wl_module_PAYLOAD);	
				}
				else
				{				
					transmitter = 0;
					wl_module_CE_lo;
					_delay_us(130);
					RX_POWERUP;
					_delay_us(130);   
					wl_module_CE_hi;
				}
			}
		}
		else
		{					
			read = bufferRead(&serialRxBuf,txBuf);
			
			if(!read)
			{
				setLed1();		
				transmitter = 1;
				wl_module_send(txBuf,DATA_PAYLOAD);	
			}
		}
		
*/

