/*
 * set_DAC_test.c
 *
 * Created: 6/16/2016 9:51:38 AM
 * Author : CJ Tulowiecki
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define LDAC 7
#define BUSY 6
#define SCK 5
#define MISO 4
#define MOSI 3
#define SS 2

#include <avr/io.h>
#include <util/delay.h>

void USART_Init( unsigned int );
void USART_Transmit( unsigned char );
unsigned char USART_Receive( void );
void SPI_MasterInit(void);
void SPI_MasterTransmit(unsigned char);

int main(void)
{
	unsigned char addr, instr0, instr1, instr2, instr3;
	
	
	// Setup
	DDRD |= (1<<LDAC);
	DDRB |= (1<<SS);
	PORTB |= (1<<SS);
	SPI_MasterInit();
	USART_Init(MYUBRR);
	
	
	
	//_delay_ms(2000);
	
    while(1)
    {	
		// Get Address and Instruction from MATLAB
		addr   = USART_Receive();
		instr0 = USART_Receive();	// lower byte
		instr1 = USART_Receive();	// middle byte
		instr2 = USART_Receive();	// upper byte
		instr3 = USART_Receive();	// don't care
		
		//_delay_ms(1000);
		
		/*USART_Transmit(instr2);
		USART_Transmit(instr1);
		USART_Transmit(instr0);*/
	
		// Write to DAC input buffers
		PORTB &= ~(1<<SS);
		SPI_MasterTransmit(instr2);
		SPI_MasterTransmit(instr1);
		SPI_MasterTransmit(instr0);
		PORTB |= (1<<SS);
		
		// Wait for NOT BUSY from DAC
		while(!(PORTD & (1<<BUSY)))
		;
		
		// Update DAC outputs
		PORTD &= ~(1<<LDAC);
		_delay_ms(1);
		PORTD |= (1<<LDAC);
		
		_delay_ms(500);
		
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
	{
		
	}
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	{
		
	}
	/* Get and return received data from buffer */
	return UDR0;
}

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output; all others input */
	DDRB |= (1<<MOSI)|(1<<SCK);
	/* Enable SPI Mode 1, Master, MSB first, 250 kHz */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<SPR1);
}

void SPI_MasterTransmit(unsigned char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}