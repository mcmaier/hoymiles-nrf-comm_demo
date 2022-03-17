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

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "hardware.h"
#include "ringbuffer.h"
#include "uart.h"

//******************************************************************************
// CONDITIONAL COMPILATION
//******************************************************************************

#ifdef SERIAL_C
#define XTRN_SERIAL
#else
#define XTRN_SERIAL extern
#endif

//******************************************************************************
// DEFINITIONS
//******************************************************************************

#define UART_TX_BUFFER_LENGTH 32
#define UART_RX_BUFFER_LENGTH 32
#define UART_RX_RINGBUFFER_ELEMENTS 4
#define UART_TX_RINGBUFFER_ELEMENTS 4


//******************************************************************************
// VARIABLES
//******************************************************************************

XTRN_SERIAL uint8_t serial__tx_buffer[UART_TX_BUFFER_LENGTH];

XTRN_SERIAL uint8_t serial__rx_buffer[UART_RX_BUFFER_LENGTH];

XTRN_SERIAL Ringbuffer_t serial__rx_ringbuffer;

XTRN_SERIAL Ringbuffer_t serial__tx_ringbuffer;

XTRN_SERIAL uint8_t serial__rx_ringbuffer_database[UART_RX_RINGBUFFER_ELEMENTS * UART_RX_BUFFER_LENGTH];

XTRN_SERIAL uint8_t serial__tx_ringbuffer_database[UART_TX_RINGBUFFER_ELEMENTS * UART_TX_BUFFER_LENGTH];


//******************************************************************************
// FUNCTION DECLARATIONS
//******************************************************************************

XTRN_SERIAL void serial__init(void);

XTRN_SERIAL void serial__put_message(uint8_t);

XTRN_SERIAL uint8_t serial__read_block(uint8_t *, uint8_t);

XTRN_SERIAL uint8_t serial__write_block(uint8_t*, uint8_t);

XTRN_SERIAL uint8_t parse_serial_input(uint8_t);



#endif


