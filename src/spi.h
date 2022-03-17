/*
    Copyright (c) 2007 Stefan Engelke <mbox@stefanengelke.de>

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

    $Id$
*/

#ifndef SPI_H_
#define SPI_H_

#ifdef SPI_C_
#define EXT_SPI
#else
#define EXT_SPI extern
#endif

#include <avr/io.h>

#define PORT_SPI    PORTB
#define DDR_SPI     DDRB
#define DD_MISO     DDB4
#define DD_MOSI     DDB3
#define DD_SS       DDB1
#define DD_SCK      DDB5

EXT_SPI void spi_init(void);
EXT_SPI void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len);
EXT_SPI void spi_transmit_sync (uint8_t * dataout, uint8_t len);
EXT_SPI uint8_t spi_fast_shift (uint8_t data);


#endif /* _SPI_H_ */
