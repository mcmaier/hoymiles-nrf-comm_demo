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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "ringbuffer.h"

//******************************************************************************
// CONDITIONAL COMPILATION
//******************************************************************************

#ifdef STATEMACHINE_C
#define XTRN_STATEMACHINE
#else
#define XTRN_STATEMACHINE extern
#endif

//******************************************************************************
// DEFINITIONS
//******************************************************************************

#define NR_OF_STATES 5

typedef enum
{
    STATE_error,
    STATE_start,
    STATE_init,
    STATE_receive,
    STATE_transmit
} States_t;

typedef enum
{
    EVENT_null,
    EVENT_timer_tick,
    EVENT_data_received,
    EVENT_data_transmit_complete,
    EVENT_data_transmit_failed

} Events_t;


//******************************************************************************
// EVENT RINGBUFFER
//******************************************************************************

#define EVENT_BUFFER_LENGTH 32
#define EVENT_BUFFER_WIDTH sizeof(Events_t)


XTRN_STATEMACHINE Ringbuffer_t ringbuffer_events;

XTRN_STATEMACHINE uint8_t ringbuffer_events__data[EVENT_BUFFER_LENGTH * EVENT_BUFFER_WIDTH];


//******************************************************************************
// FUNCTION DECLARATIONS
//******************************************************************************

XTRN_STATEMACHINE States_t state_machine__init(void);
XTRN_STATEMACHINE States_t state_machine__calculate_state(Events_t);

XTRN_STATEMACHINE uint8_t event_push(Events_t);
XTRN_STATEMACHINE uint8_t event_get(Events_t*);

//******************************************************************************
// STATE FUNCTIONS
//******************************************************************************

XTRN_STATEMACHINE States_t STATE_error__handler(Events_t);
XTRN_STATEMACHINE States_t STATE_start__handler(Events_t);
XTRN_STATEMACHINE States_t STATE_init__handler(Events_t);
XTRN_STATEMACHINE States_t STATE_receive__handler(Events_t);
XTRN_STATEMACHINE States_t STATE_transmit__handler(Events_t);

//******************************************************************************
// STATE ENTERING FUNCTIONS
//******************************************************************************

XTRN_STATEMACHINE void STATE_error__entering_handler(void);
XTRN_STATEMACHINE void STATE_start__entering_handler(void);
XTRN_STATEMACHINE void STATE_init__entering_handler(void);
XTRN_STATEMACHINE void STATE_receive__entering_handler(void);
XTRN_STATEMACHINE void STATE_transmit__entering_handler(void);


#endif /* STATEMACHINE_H_ */