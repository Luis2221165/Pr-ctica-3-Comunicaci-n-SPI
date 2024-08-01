#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

typedef enum
{
	SPI_MODE_MASTER_DIV2    = 0b01010000,
	SPI_MODE_MASTER_DIV4    = 0b01010001,
	SPI_MODE_MASTER_DIV8    = 0b01010010,
	SPI_MODE_MASTER_DIV16   = 0b01010011,
	SPI_MODE_MASTER_DIV32   = 0b01010100,
	SPI_MODE_MASTER_DIV64   = 0b01010101,
	SPI_MODE_MASTER_DIV128  = 0b01010110,
	SPI_MODE_SLAVE          = 0b01000000,
} Spi_Mode;

typedef enum
{
	SPI_MSB_FIRST           = 0b00000000,
	SPI_LSB_FIRST           = 0b00100000,
} Spi_DataOrder;

typedef enum
{
	SPI_IDLE_HIGH           = 0b00001000,
	SPI_IDLE_LOW            = 0b00000000,
} Spi_ClockPolarity;

typedef enum
{
	SPI_FIRST_EDGE          = 0b00000000,
	SPI_SECOND_EDGE         = 0b00000100,
} Spi_ClockPhase;

void spiInit(Spi_Mode mode, Spi_DataOrder dataOrder, Spi_ClockPolarity clockPolarity, Spi_ClockPhase clockPhase);
void spiSend(uint8_t data);
unsigned spiIsDataReady();
uint8_t spiReceive(void);

#endif /* MY_SPI_H_ */
