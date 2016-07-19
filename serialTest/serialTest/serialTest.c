/*
 * serialTest.c
 *
 * Created: 6/2/2016 3:35:21 PM
 *  Author: tulowieckic
 * Description: This program has 4 functions, 3 of them related to the AVR's USART.
 * The 4th function is char2float, which takes 4, 8-bit chars and concatenates them into 
 * 1 32-bit float. When combined with the MATLAB program serialTest.m, the AVR will 
 * receive a 32-bit float from MATLAB, in 8-bit pieces. The C program will store each
 * of the 8-bit transmissions in a different variable and then pass them to the 
 * char2float function. The result is stored in the variable 'data'. Finally,
 * to verify that the conversion was successful, 'data' is multiplied by 2
 * (which in this case gives an even integer) and is cast as a char to be
 * sent via the USART to MATLAB. If successful, give a float of 1.5, the 
 * program will return (via USART) the 8-bit value 3.
 */ 

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>

void USART_Init( unsigned int );
void USART_Transmit( unsigned char );
unsigned char USART_Receive( void );
float char2float( unsigned char, unsigned char, unsigned char, unsigned char );

int main(void)
{
	unsigned char data0, data1, data2, data3;
	float data;
	unsigned char dataNew;
	
	USART_Init(MYUBRR);
	
    while(1)
    {
		data0 = USART_Receive();
		data1 = USART_Receive();
		data2 = USART_Receive();
		data3 = USART_Receive();
		
		data = char2float(data0, data1, data2, data3);
		dataNew = (unsigned char)(data*2);
		USART_Transmit(dataNew);
		
    }
}

void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

float char2float( unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3 )
{
	float output;
	
	*((unsigned char*)(&output) + 3 * sizeof(unsigned char)) = byte3;
	*((unsigned char*)(&output) + 2 * sizeof(unsigned char)) = byte2;
	*((unsigned char*)(&output) + 1 * sizeof(unsigned char)) = byte1;
	*((unsigned char*)(&output) + 0 * sizeof(unsigned char)) = byte0;
	
	return output;
}