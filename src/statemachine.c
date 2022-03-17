
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

#ifndef STATEMACHINE_C
#define STATEMACHINE_C

#include "statemachine.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
// VARIABLES
//******************************************************************************

static bool state_machine__initialized;
States_t state_machine__current_state;

//******************************************************************************
// FUNCTION POINTERS
//******************************************************************************

static States_t (*p_state_machine__state_function[NR_OF_STATES])(Events_t);
static void     (*p_state_machine__state_entering_function[NR_OF_STATES])(void);

//******************************************************************************
// FUNCTION DECLARATIONS
//******************************************************************************

States_t state_machine__init(void);

States_t state_machine__calculate_state(Events_t);

//******************************************************************************
// FUNCTIONS
//******************************************************************************

// state_machine__init
// Initialize the state machine and the event ringbuffer
//------------------------------------------------------------------------------

States_t state_machine__init(void)
{
  ringbuffer__init(&ringbuffer_events, ringbuffer_events__data, EVENT_BUFFER_LENGTH, EVENT_BUFFER_WIDTH);

  //Initialize function pointers
  p_state_machine__state_function[0] = &STATE_error__handler;
  p_state_machine__state_function[1] = &STATE_start__handler;
  p_state_machine__state_function[2] = &STATE_init__handler;
  p_state_machine__state_function[3] = &STATE_receive__handler;
  p_state_machine__state_function[4] = &STATE_transmit__handler;

  p_state_machine__state_entering_function[0] = &STATE_error__entering_handler;
  p_state_machine__state_entering_function[1] = &STATE_start__entering_handler;
  p_state_machine__state_entering_function[2] = &STATE_init__entering_handler;
  p_state_machine__state_entering_function[3] = &STATE_receive__entering_handler;
  p_state_machine__state_entering_function[4] = &STATE_transmit__entering_handler;

  state_machine__current_state = STATE_start;
  state_machine__initialized = true;

  return state_machine__current_state;
}

//states_t state_machine__calculate_state(events_t event)
//Calculates the new state depending on the input event
//Returns: Next state
//------------------------------------------------------------------------------
States_t state_machine__calculate_state(Events_t event)
{
  States_t new_state = state_machine__current_state;

  if(!state_machine__initialized)
  {
    return STATE_error;
  }
  else
  {
    //Call state function
    new_state = p_state_machine__state_function[(uint8_t)state_machine__current_state](event);

    //If state has changed, call new state entering function
    if (new_state != state_machine__current_state)
    {
      p_state_machine__state_entering_function[(uint8_t)new_state]();
      state_machine__current_state = new_state;
    }

    return state_machine__current_state;
  }   
}

// event_push
// Pushes an event into the ringbuffer
//------------------------------------------------------------------------------
uint8_t event_push(Events_t event)
{
    Events_t temp_event = event;
    Ringbuffer_State_t bufstate = ringbuffer__push(&ringbuffer_events,(uint8_t*)&temp_event);

    if((bufstate == BUFFER_normal) || (bufstate == BUFFER_full))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// event_get
// Gets an event out of the ringbuffer by reference
//------------------------------------------------------------------------------
uint8_t event_get(Events_t * p_event)
{
    Ringbuffer_State_t bufstate = ringbuffer__pop(&ringbuffer_events, (uint8_t *)p_event);

    if((bufstate == BUFFER_normal) || (bufstate == BUFFER_empty))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


#endif /*STATEMACHINE_C*/