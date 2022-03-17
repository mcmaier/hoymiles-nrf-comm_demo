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
#include <avr/pgmspace.h>

#include "hardware.h"
#include "statemachine.h"
#include "uart.h"
#include "serial.h"
#include "messaging.h"
//#include "ringbuffer.h"

//#include "calculation.h"
//#include "uart.h"
//#include "spi.h"
//#include "nRF24L01.h"
//#include "wl_module.h"

extern volatile uint8_t PTX;
static uint32_t timer;
static uint8_t init_step;

extern uint32_t sys_timer;

//static uint8_t payload[16];

#define INIT_TIMEOUT	3000

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
				if(timer++ == 1)					
				{
					LED1_ON;
					LED2_ON;

					serial__put_message(0);
					serial__put_message(1);
				}
				else if(timer == 50)
				{
					LED1_OFF;
					LED2_OFF
					ret_state = STATE_init;
				}

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
	uint8_t parse_success;	

	switch(event)
	{	
		case EVENT_timer_tick:							
				if(timer++ == 1)					
				{
					parse_success = 0xFF;
					LED1_ON;
					switch(init_step)
					{
						case 0: serial__put_message(2);
						break;

						case 1: serial__put_message(3);
						break;

						case 2: serial__put_message(4);
						break;

						case 3: serial__put_message(5);
						break;

						default:
						break;
					}					
				}					
				else if((timer > 1) && (timer < INIT_TIMEOUT))
				{	
					parse_success = parse_serial_input(init_step);

					if(parse_success == init_step)
					{
						uart_puts_P("\r\n");
						timer = 0;

						if(++init_step > 3)
						{
							serial__put_message(1);
							LED2_OFF;
							LED1_ON;						
							ret_state = STATE_receive;
						}
					}
				}
				else if(timer >= INIT_TIMEOUT)
				{
					uart_puts_P("\r\n");
					serial__put_message(6);
					LED2_ON;
					ret_state = STATE_error;
				}	

				if((timer & 0x7F) == 0x01)	
				{
					LED1_TOGGLE;					
				}														
			break;

		case EVENT_data_received:
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
	
	
	switch(event)
	{	
		case EVENT_timer_tick:
			if(timer++ > 500)
			{
				timer = 0;
				ret_state = STATE_transmit;
			}

		break;

		default:
		break;
	}

	return ret_state;
}				
/*
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

//----------------------------------------------------------
// TRANSMIT STATE HANDLER
//---------------------------------------------------------

States_t STATE_transmit__handler(Events_t event)
{
	States_t ret_state = STATE_transmit;

	switch(event)
	{	
		case EVENT_timer_tick:	
			if(timer++ == 1)
			{
				LED2_ON;
				message_builder(&tx_message);

				uart_putc_arr(tx_message.message_buffer,tx_message.message_type.msg_data_length + 11);
				uart_puts_P("\r\n");
			}
			else if(timer == 10)
			{
				LED2_OFF;
				ret_state = STATE_receive;
			}
		break;

		default:
		break;
	}

	return ret_state;
}
/*
				
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
	init_step = 0;
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
	tx_message.command = 0x15;
	memcpy(tx_message.tx_address,dtu_address,4);
	memcpy(tx_message.rx_address,inv_address,4);	
	tx_message.message_type.msg_packet_id = DTU_DATETIME__PACKET_ID;
	tx_message.message_type.msg_data_length = DTU_DATETIME__DATA_LENGTH;
}
