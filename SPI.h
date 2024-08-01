#include "SPI.h"

void spiInit(Spi_Type mode, Spi_Data_Order dataOrder, Spi_Clock_Polarity clockPolarity, Spi_Clock_Phase clockPhase)
{
	// PB2 -> D10 -> SS
	// PB3 -> D11 -> MOSI
	// PB4 -> D12 -> MISO
	// PB5 -> D13 -> SCK

	if(mode & (1<<MSTR)) // Si es modo maestro
	{
		DDRB |= (1<<DDB3) | (1<<DDB5) | (1<<DDB2); // MOSI, SCK, SS como salida
		DDRB &= ~(1<<DDB4); // MISO como entrada
		SPCR |= (1<<MSTR); // Configurar como maestro

		uint8_t clockDiv = mode & 0b00000111;
		switch(clockDiv){
			case 0: // DIV2
			SPCR &= ~((1<<SPR1) | (1<<SPR0));
			SPSR |= (1<<SPI2X);
			break;
			case 1: // DIV4
			SPCR &= ~((1<<SPR1) | (1<<SPR0));
			SPSR &= ~(1<<SPI2X);
			break;
			case 2: // DIV8
			SPCR |= (1<<SPR0);
			SPCR &= ~(1<<SPR1);
			SPSR |= (1<<SPI2X);
			break;
			case 3: // DIV16
			SPCR |= (1<<SPR0);
			SPCR &= ~(1<<SPR1);
			SPSR &= ~(1<<SPI2X);
			break;
			case 4: // DIV32
			SPCR &= ~(1<<SPR0);
			SPCR |= (1<<SPR1);
			SPSR |= (1<<SPI2X);
			break;
			case 5: // DIV64
			SPCR &= ~(1<<SPR0);
			SPCR |= (1<<SPR1);
			SPSR &= ~(1<<SPI2X);
			break;
			case 6: // DIV128
			SPCR |= ((1<<SPR1) | (1<<SPR0));
			SPSR &= ~(1<<SPI2X);
			break;
		}
	}
	else // Si es modo esclavo
	{
		DDRB |= (1<<DDB4); // MISO como salida
		DDRB &= ~((1<<DDB3) | (1<<DDB5) | (1<<DDB2)); // MOSI, SCK, SS como entrada
		SPCR &= ~(1<<MSTR); // Configurar como esclavo
	}

	// Configurar orden de datos, polaridad de reloj y fase de reloj
	SPCR |= (1<<SPE) | dataOrder | clockPolarity | clockPhase;
}

void spiSend(uint8_t data) // Escribir datos en el bus SPI
{
	SPDR = data;
}

unsigned spiIsDataReady() // Verificar si los datos están listos para leer
{
	return (SPSR & (1<<SPIF)) ? 1 : 0;
}

uint8_t spiReceive(void)
{
	while (!(SPSR & (1<<SPIF))); // Esperar hasta que la recepción de datos esté completa
	return SPDR; // Leer los datos recibidos del buffer
}
