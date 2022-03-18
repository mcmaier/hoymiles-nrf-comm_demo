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

#include <avr/io.h>

#ifndef HARDWARE_H_
#define HARDWARE_H_

#ifdef HARDWARE_C_
#define XTRN_HW
#else
#define XTRN_HW extern
#endif

/* NO LEDs on Arduino available
#define LED1_PIN	PC0
#define LED1_PORT	PORTC
#define LED1_DDR	DDRC

#define LED2_PIN	PC1
#define LED2_PORT	PORTC
#define LED2_DDR	DDRC
*/

#define LED1_ON			//LED1_PORT |= (1<<LED1_PIN);
#define LED1_OFF		//LED1_PORT &= ~(1<<LED1_PIN);
#define LED1_TOGGLE		//if(LED1_PORT & (1 << LED1_PIN)) LED1_OFF else LED1_ON

#define LED2_ON			//LED2_PORT |= (1<<LED2_PIN);
#define LED2_OFF		//LED2_PORT &= ~(1<<LED2_PIN);
#define LED2_TOGGLE		//if(LED2_PORT & (1 << LED2_PIN)) LED2_OFF else LED2_ON


XTRN_HW void 		hardware_init(void);

#endif /* HARDWARE_H_ */