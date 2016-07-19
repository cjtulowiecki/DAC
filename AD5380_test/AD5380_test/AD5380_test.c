/*
 * AD5380_test.c
 *
 * Created: 6/10/2016 2:53:39 PM
 * Author : CJ Tulowiecki
 */ 

#define F_CPU 16000000UL
#define LDAC 7
#define BUSY 6
#define SCK 5
#define MISO 4
#define MOSI 3
#define SS 2

#include <avr/io.h>
#include <util/delay.h>

void SPI_MasterInit(void);
void SPI_MasterTransmit(char);

int main(void)
{
	// Setup
	DDRD |= (1<<LDAC);
	DDRB |= (1<<SS);
	PORTB |= (1<<SS);
	SPI_MasterInit();
	
	// Message - Set 2.5V on Channel 10
	char packet0 = 0x0A;
	char packet1 = 0xE0;
	char packet2 = 0x00;
		 
	/*char packet3 = 0x0B;
	char packet4 = 0xE0;
	char packet5 = 0x00;
	
	char packet6 = 0x0C;
	char packet7 = 0xE0;
	char packet8 = 0x00;*/
	
	// Wait 2 Seconds
	_delay_ms(2000); 
	
	while(1)
	{
		// Write to input buffers
		PORTB &= ~(1<<SS);
		SPI_MasterTransmit(packet0);
		SPI_MasterTransmit(packet1);
		SPI_MasterTransmit(packet2);
		PORTB |= (1<<SS);
		
		/*// Write to input buffers
		PORTB &= ~(1<<SS);
		SPI_MasterTransmit(packet3);
		SPI_MasterTransmit(packet4);
		SPI_MasterTransmit(packet5);
		PORTB |= (1<<SS);
		
		// Write to input buffers
		PORTB &= ~(1<<SS);
		SPI_MasterTransmit(packet6);
		SPI_MasterTransmit(packet7);
		SPI_MasterTransmit(packet8);
		PORTB |= (1<<SS);*/
			
		// Wait for NOT BUSY
		while(!(PORTD & (1<<BUSY)))
			;
			
		// Update DAC outputs
		PORTD &= ~(1<<LDAC);
		_delay_ms(1);
		PORTD |= (1<<LDAC);
		
		_delay_ms(500);	
	}
}

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output; all others input */
	DDRB |= (1<<MOSI)|(1<<SCK);
	/* Enable SPI Mode 1, Master, MSB first, 250 kHz */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<SPR1);
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
		;
}