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

#ifndef SERIAL_C
#define SERIAL_C

#include "serial.h"
#include "uart.h"

#include <avr/pgmspace.h>
#include <stdlib.h>

//******************************************************************************
// INTERNAL VARIABLES
//******************************************************************************

static bool serial__tx_busy;

static uint8_t writebuf[UART_TX_BUFFER_LENGTH];

extern uint32_t sys_timer;

//Debug messages
//-----------------------------------------------------------------------------

#define MESSAGES 8

uint8_t const message_buffer[MESSAGES][UART_TX_BUFFER_LENGTH] PROGMEM =
    {
        "Hoymiles NRF Interface\r\n",
        "----------------------\r\n",
        "Enter date YYYYMMDD: ",
        "Enter time HHMMSS: ",
        "Enter DTU serial(last 8 digt): ",
        "Enter INV serial(last 8 digt): ",
        "Init incomplete\r\n",
        ""
      };

//******************************************************************************
// FUNCTIONS
//******************************************************************************

//serial__init
//------------------------------------------------------------------------------
void serial__init(void)
{
  // Basic Settings
  //---------------

  //Initialize Receive FIFO
  ringbuffer__init(&serial__rx_ringbuffer, serial__rx_ringbuffer_database, UART_RX_RINGBUFFER_ELEMENTS, UART_RX_BUFFER_LENGTH);

  //Initialize Transmit FIFO
  ringbuffer__init(&serial__tx_ringbuffer, serial__tx_ringbuffer_database, UART_TX_RINGBUFFER_ELEMENTS, UART_TX_BUFFER_LENGTH);

  serial__tx_busy = false;
}

//serial__init
//------------------------------------------------------------------------------
uint8_t serial__write_block(uint8_t *data, uint8_t nBytes)
{ 

  uart_putc_arr(data, nBytes);

  return 0;
}

//void serial__put_message(uint8_t message_nr)
// Puts one of the predefined messages to UART
//------------------------------------------------------------------------------

void serial__put_message(uint8_t message_nr)
{
  if(message_nr < MESSAGES)
  {
    uart_puts_p(&message_buffer[message_nr]);
  }  
}

//void serial__putparse_serial_input_message(uint8_t message_nr)
//------------------------------------------------------------------------------

/*
tm_sec + tm_min*60 + tm_hour*3600 + tm_yday*86400 +
    (tm_year-70)*31536000 + ((tm_year-69)/4)*86400 -
    ((tm_year-1)/100)*86400 + ((tm_year+299)/400)*86400
*/

uint8_t parse_serial_input(uint8_t parse_nr)
{
  uint8_t nr_of_bytes;
  uint8_t ret_value = 0xFF;
  uint8_t rx_buffer[16];

  volatile uint32_t temp;

  nr_of_bytes = uart_available();

  if(nr_of_bytes)
  {
    memset(rx_buffer,0,16);
    
    for(uint8_t i = 0; i < nr_of_bytes; ++i)
    {
      rx_buffer[i] = (uint8_t)(uart_getc() & 0xFF);
    }

    switch (parse_nr)
    {
      case 0: 
        // Get year        
        temp =  (rx_buffer[0] - '0')*1000 + (rx_buffer[1] - '0')*100 + (rx_buffer[2] - '0')*10 + (rx_buffer[3] - '0');
        // Add years to sys tick
         sys_timer = (temp - 1970)*31556926;

        // Get month        
        temp =  (rx_buffer[4] - '0')*10 + (rx_buffer[5] - '0');
        // Add months to sys tick
         sys_timer += temp*2629743; 

        // Get day        
        temp =  (rx_buffer[6] - '0')*10 + (rx_buffer[7] - '0');
        // Add days to sys tick
        sys_timer += temp*86400;   

        uart_putc_arr(rx_buffer,nr_of_bytes);

        ret_value = 0;
        
        break;

      case 1:      
       // Get hours        
        temp =  (rx_buffer[0] - '0')*10 + (rx_buffer[1] - '0');
        sys_timer += temp*3600;

        // Get minutes        
        temp =  (rx_buffer[2] - '0')*10 + (rx_buffer[3] - '0');
        sys_timer += temp*60; 

        // Get seconds        
        temp =  (rx_buffer[4] - '0')*10 + (rx_buffer[5] - '0');
        sys_timer += temp; 

        uart_putc_arr(rx_buffer,nr_of_bytes);
        ret_value = 1;
        break;

      case 2:

      ((uint8_t*)&temp)[0] =  (rx_buffer[0] - '0')*16 + (rx_buffer[1] - '0');
      ((uint8_t*)&temp)[1] =  (rx_buffer[2] - '0')*16 + (rx_buffer[3] - '0');
      ((uint8_t*)&temp)[2] =  (rx_buffer[4] - '0')*16 + (rx_buffer[5] - '0');
      ((uint8_t*)&temp)[3] =  (rx_buffer[6] - '0')*16 + (rx_buffer[7] - '0');

      messaging_set_dtu_address((uint8_t*) &temp);
      uart_putc_arr(rx_buffer,nr_of_bytes);
      ret_value = 2;
      break;

      case 3:
      ((uint8_t*)&temp)[0] =  (rx_buffer[0] - '0')*16 + (rx_buffer[1] - '0');
      ((uint8_t*)&temp)[1] =  (rx_buffer[2] - '0')*16 + (rx_buffer[3] - '0');
      ((uint8_t*)&temp)[2] =  (rx_buffer[4] - '0')*16 + (rx_buffer[5] - '0');
      ((uint8_t*)&temp)[3] =  (rx_buffer[6] - '0')*16 + (rx_buffer[7] - '0');

      messaging_set_inv_address((uint8_t*) &temp);
      uart_putc_arr(rx_buffer,nr_of_bytes);
      ret_value = 3;
      break;

      default:
        break;
    }
  }  

  return ret_value;
}



#endif //SERIAL
