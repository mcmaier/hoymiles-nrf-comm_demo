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

#ifndef HARDWARE_C_
#define HARDWARE_C_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "hardware.h"
#include "uart.h"
#include "wl_module.h"

void hardware_init(void)
{
	//Set pullup
	PORTD |= (1 << PD2);

	//Configure pins as output
	DDRB |= ((1 << PB0)|(1 << PB2)|(1 << PB3)|(1 << PB5));
	DDRD |= (1 << PD1);
	
	LED1_DDR |= (1 << LED1_PIN);
	LED2_DDR |= (1 << LED2_PIN);	
	
	//10 ms Timer
	
	OCR1A =	96;

	TCCR1B = (1 << WGM12)|(1 << CS12)|(1 << CS10);		
	TIMSK1 = (1 << OCIE1A);
	
	//External Interrupt
	EIMSK = (1 << INT0);
	
	//Turn of analog comparator
    ACSR = 0x80;

	//Init UART
	//Baudrate = 115200 bps = 10 @ 10 MHz, U2X = 1
	//Baudrate = 57600 bps = 10 @ 10 MHz, U2X = 0
	UBRR0 = 10;		
	UCSR0A |= (1 << U2X0);	
	UCSR0B = (1 << RXCIE0)|(1 << RXEN0)|(1 << TXEN0); 
}	

#endif