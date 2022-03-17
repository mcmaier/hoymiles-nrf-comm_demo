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

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//******************************************************************************
// CONDITIONAL COMPILATION
//******************************************************************************

#ifdef RINGBUFFER_C
#define XTRN_BUFFER
#else
#define XTRN_BUFFER extern
#endif

//******************************************************************************
// DEFINITIONS
//******************************************************************************

typedef enum
{
  BUFFER_invalid,
  BUFFER_empty,
  BUFFER_normal,
  BUFFER_full,
  BUFFER_overflow,
  BUFFER_underrun
} Ringbuffer_State_t;

//Ringbuffer meta information struct

typedef struct
{  
  Ringbuffer_State_t    state;
  uint8_t               read_pos;
  uint8_t               write_pos;
  uint8_t               size;
  uint8_t               width;
  uint8_t               busy;  
  uint8_t*              p_buffer_data;
} Ringbuffer_t;

//******************************************************************************
// VARIABLES
//******************************************************************************

//******************************************************************************
// FUNCTION DECLARATIONS
//******************************************************************************

XTRN_BUFFER Ringbuffer_State_t ringbuffer__init(Ringbuffer_t*, uint8_t*, uint8_t, uint8_t);

XTRN_BUFFER Ringbuffer_State_t ringbuffer__push(Ringbuffer_t *, uint8_t *);

XTRN_BUFFER Ringbuffer_State_t ringbuffer__pop(Ringbuffer_t*, uint8_t*);

XTRN_BUFFER Ringbuffer_State_t ringbuffer__flush(Ringbuffer_t *);

XTRN_BUFFER Ringbuffer_State_t ringbuffer__get_state(Ringbuffer_t*);

XTRN_BUFFER uint8_t ringbuffer__contains(Ringbuffer_t*, void*);


#endif
