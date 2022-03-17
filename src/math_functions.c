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

#ifndef MATHFUNCTIONS_C
#define MATHFUNCTIONS_C

#include "math_functions.h"

//******************************************************************************
// VARIABLES
//******************************************************************************

const double PI = 3.14159265;

//******************************************************************************
// INTERNAL FUNCTION DECLARATION
//******************************************************************************

//Swap two numbers
void swap(uint8_t *, uint8_t *);

//Reverse order of buffer
uint8_t* reverse(uint8_t *buffer, int8_t, int8_t);

//******************************************************************************
// FUNCTIONS
//******************************************************************************

//Get absolute value of a number
//------------------------------------------------------------------------------
int32_t absolute(int32_t in)
{
  if( in < 0)
  {
    return -in;
  }
  else
  {
    return in;
  }
}

//Get sign of a number
//------------------------------------------------------------------------------
int32_t signum(int32_t in)
{
  if(in < 0)
  {
    return -1;
  }
  else if(in == 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

//Approximation of square root
//------------------------------------------------------------------------------
uint32_t sqrt(uint32_t val) 
{
    uint32_t temp;
    uint32_t g = 0;
    uint32_t b = 0x8000;
    uint32_t bshft = 15;
    int32_t  i;
    
    for (i = 0; i < 16; i++) 
    {
      temp = g << 1;
      temp = temp + b;
      temp = temp << bshft;
      if ( val >= temp)
      {
        g += b;
        val -= temp;
      }
      bshft--;
      b = b >> 1;
    }
    
    return g;
}

//Swap to numbers
//------------------------------------------------------------------------------
void swap(uint8_t *x, uint8_t *y) 
{
	uint8_t t = *x; *x = *y; *y = t;
}

//Reverse a buffer
//------------------------------------------------------------------------------
uint8_t* reverse(uint8_t *buffer, int8_t i, int8_t j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

//Iterative itoa implementation
//------------------------------------------------------------------------------
uint8_t* itoa(int32_t value, uint8_t* buffer, int8_t base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int32_t n = absolute(value);

	int16_t i = 0;
	while (n)
	{
		int32_t r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

//Simple char to hex_string[2] conversion
//------------------------------------------------------------------------------
void char_to_hex(uint8_t input, uint8_t *outbuf)
{
  #define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

  outbuf[0] = TO_HEX(((input & 0x00F0) >> 4));
  outbuf[1] = TO_HEX((input & 0x000F));
}

//Convert a byte to a 0xFF/0x00 byte array for one wire communication
//------------------------------------------------------------------------------
void bits_to_bytes(uint8_t input, uint8_t* outbuf)
{
  uint8_t mask = 0x01;

  for(uint8_t i = 0; i < 8; ++i)
  {
    if (input & (mask << i))
    {
      outbuf[i] = 0xFF;
    }
    else
    {
      outbuf[i] = 0x00;
    }    
  }
}

//Convert a byte array of hex values to one byte for one wire communication
//------------------------------------------------------------------------------
uint8_t bytes_to_bits(uint8_t* inbuf, uint8_t threshold)
{
  uint8_t out = 0x00;
  uint8_t mask = 0x01;

  for (uint8_t i = 0; i < 8; ++i)
  {
    if (inbuf[i] > threshold)
    {
      out = out | (mask << i);
    }
  }

  return out;
}

#endif
