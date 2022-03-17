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

#ifndef RINGBUFFER_C
#define RINGBUFFER_C

#include "ringbuffer.h"

//******************************************************************************
// INTERNAL VARIABLES
//******************************************************************************

//******************************************************************************
// FUNCTIONS
//******************************************************************************

//Ringbuffer_State_t ringbuffer__init(Ringbuffer_t *buffer, uint8_t *p_array, uint8_t buf_size, uint8_t buf_width)
//Initializes a buffer with the selected values 
//------------------------------------------------------------------------------
Ringbuffer_State_t ringbuffer__init(Ringbuffer_t *buffer, uint8_t *p_array, uint8_t buf_size, uint8_t buf_width)
{
  if((buf_width != 1) || (buf_width != 2) || (buf_width != 4))
  {
    buffer->state = BUFFER_empty;  
    buffer->write_pos = 0;
    buffer->read_pos = 0;
    buffer->busy = 0;
    buffer->size = buf_size;
    buffer->width = buf_width;
    buffer->p_buffer_data = p_array;
  }
  else
  { 
    buffer->state = BUFFER_invalid;  
  }

  return buffer->state;
}

//Ringbuffer_State_t  ringbuffer__push(Ringbuffer_t* buffer, uint8_t* data)
//Pushes an element into a buffer
//Returns state of buffer, Overflow in case buffer is full
//------------------------------------------------------------------------------
Ringbuffer_State_t  ringbuffer__push(Ringbuffer_t* buffer, uint8_t* data)
{   
  //Check if buffer is full
  if(buffer->state == BUFFER_full)
  {
    return BUFFER_overflow;
  }
  else if(buffer->state == BUFFER_invalid)
  {
    return BUFFER_invalid;
  }
  else
  {
    buffer->busy = 1;
    memcpy((void *)(&buffer->p_buffer_data[buffer->write_pos * buffer->width]), (void *)data, buffer->width);
    buffer->write_pos = (buffer->write_pos + 1) & (buffer->size - 1);
    
    if(buffer->write_pos == buffer->read_pos)
    {
      buffer->state = BUFFER_full;
    }
    else
    {
      buffer->state = BUFFER_normal;
    }
    
    buffer->busy = 0;
           
    return buffer->state;
  }
}

//Ringbuffer_State_t ringbuffer__pop(Ringbuffer_t *buffer, uint8_t *data)
//Gets an element out of the buffer
//------------------------------------------------------------------------------
Ringbuffer_State_t ringbuffer__pop(Ringbuffer_t *buffer, uint8_t *data)
{  
  //Return error if buffer is empty
  if(buffer->state == BUFFER_empty)
  {
    return BUFFER_underrun;
  }
  else if(buffer->state == BUFFER_invalid)
  {
    return BUFFER_invalid;
  }
  else
  {
    buffer->busy = 1;

    memcpy((void *)data, (void *)(&buffer->p_buffer_data[buffer->read_pos * buffer->width]), buffer->width);
    buffer->read_pos = (buffer->read_pos + 1) & (buffer->size - 1);

    //Wenn Leseposition = Schreibposition
    if(buffer->read_pos == buffer->write_pos)
    {
      //Pufferzustand leer
      buffer->state = BUFFER_empty;
    }
    //Sonst Pufferzustand normal
    else
    {
      buffer->state = BUFFER_normal;
    }
    
    buffer->busy = 0;

    return buffer->state;
  }  
}

// Ringbuffer_State_t  ringbuffer__flush(Ringbuffer_t* buffer)
// Clears all items of the buffer
//------------------------------------------------------------------------------
Ringbuffer_State_t ringbuffer__flush(Ringbuffer_t *buffer)
{
  if(buffer->state == BUFFER_invalid)
  {
    return BUFFER_invalid;
  }
  else
  {
    buffer->busy = 1;
    buffer->write_pos = buffer->read_pos;
    buffer->state = BUFFER_empty;
    buffer->busy = 0;

    return buffer->state;
  }
}

// Ringbuffer_State_t  ringbuffer__get_state(Ringbuffer_t* buffer)
// returns the state of the ringbuffer
//------------------------------------------------------------------------------
Ringbuffer_State_t  ringbuffer__get_state(Ringbuffer_t* buffer)
{ 
  return buffer->state;
}

// bool ringbuffer__contains_element(Ringbuffer_t, uint8_t * )
// returns true, if the ringbuffer contains the element at least one time
//------------------------------------------------------------------------------
uint8_t ringbuffer__contains(Ringbuffer_t * buffer, void* compare_element)
{
  uint8_t count = 0;
  uint8_t read_offset = 0;

  //Wenn kein Element vorhanden,
  if ((buffer->state == BUFFER_empty) || (buffer->state == BUFFER_invalid))
  {
    return 0;
  }
  else
  {
    buffer->busy = 1;

    do
    {
      switch(buffer->width)
      {
        case 1:
          if (*((uint8_t*)compare_element) == *((uint8_t*)(&buffer->p_buffer_data[(buffer->read_pos + read_offset)* buffer->width])))
          {
            count++;
          }
          break;

        case 2:
          if (*((uint16_t*)compare_element) == *((uint16_t *)(&buffer->p_buffer_data[(buffer->read_pos + read_offset) * buffer->width])))
          {
            count++;
          }
        break;

        case 4:
          if (*((uint32_t*)compare_element) == *((uint32_t *)(&buffer->p_buffer_data[(buffer->read_pos + read_offset) * buffer->width])))
          {
            count++;
          }
        break;

        default:
        break;
      }
      
    } while ((++read_offset & (buffer->size - 1)) != buffer->write_pos);

    buffer->busy = 0;
  }
  
  return count;
}

#endif
