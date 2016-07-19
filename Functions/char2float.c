#include "char2float.h"

/*******************************************************************************
Name: char2float
Description: char2float returns a 32-bit float given four 8-bit chars. The four
	8-bit chars must be the four bytes which make up the binary IEEE 754
	floating point format of 32-bit float. For example, let the MATLAB fwrite
	function be used to transmit a 'float' precision value 1.5 to a serial
	device, using 8 data bits per transmission. The IEEE 754 floating point
	repesentation is as follows: 0x3FC00000. What the MATLAB function will
	transmit is four transmissions in this order: 0x00 (byte 0), 0x00 (byte 1),
	0xC0 (byte 2), 0x3F (byte 3). In order to 'rebuild' this number on the 8-bit
	ATMEGA328P, you mustconcatenate the four bytes using this function call:
	char2float( byte0, byte1, byte2, byte3 ).
Input:
	byte0 - an unsigned char representing bits 7:0
	byte1 - an unsigned char representing bits 15:8
	byte2 - an unsigned char representing bits 23:16
	byte3 - an unsigned char representing bits 31:24
Output:
	return a float in the 32-bit IEEE 754 floating point format.
*******************************************************************************/
float char2float(unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3)
{
	float output;

	*((unsigned char*)(&output) + 3 * sizeof(unsigned char)) = byte3;
	*((unsigned char*)(&output) + 2 * sizeof(unsigned char)) = byte2;
	*((unsigned char*)(&output) + 1 * sizeof(unsigned char)) = byte1;
	*((unsigned char*)(&output) + 0 * sizeof(unsigned char)) = byte0;

	return output;
}