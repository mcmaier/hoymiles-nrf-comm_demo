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

#ifndef CALCULATION_H
#define CALCULATION_H

#include <inttypes.h>
#include <stdbool.h>

//******************************************************************************
// CONDITIONAL COMPILATION
//******************************************************************************

#ifdef CALCULATION_C
#define XTRN_CALCULATION
#else
#define XTRN_CALCULATION extern
#endif

//******************************************************************************
// FUNCTION DECLARATIONS
//******************************************************************************

/*
XTRN_CALC void sendDataStruct(uint8_t, uint8_t*);

XTRN_CALC void readReceiveBuffer(void);

XTRN_CALC void pollSerialBuffer(void);

XTRN_CALC void parseData(uint8_t*, uint8_t*);
*/

#endif /* CALCULATION_H */