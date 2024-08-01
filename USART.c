#include <avr/io.h>
#include <stdint.h>
#include "UART.h"

void uartInit9600(void) {
	// Paso 1: Configurar RX como entrada y TX como salida
	DDRD &= ~(1<<DDD0); // RX como entrada
	DDRD |= (1<<DDD1);  // TX como salida

	// Paso 2: Configurar UCSR0A
	UCSR0A = 0;  // Resetear registro

	// Paso 3: Configurar UCSR0B habilitando interrupciones de RX, recepci�n y transmisi�n
	UCSR0B = 0;  // Resetear registro
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);  // Habilitar interrupciones de RX, recepci�n y transmisi�n

	// Paso 4: Configurar UCSR0C para comunicaci�n as�ncrona, sin paridad, 1 bit de stop, 8 bits de datos
	UCSR0C = 0;  // Resetear registro
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);  // Configurar 8 bits de datos

	// Paso 5: Configurar baudrate a 9600
	UBRR0 = 103;
}

void uartWriteChar(char character) {
	// Esperar a que el buffer est� vac�o
	while (!(UCSR0A & (1<<UDRE0)));

	// Colocar el dato en el buffer de transmisi�n
	UDR0 = character;
}

void uartWriteString(char* text) {
	uint8_t i = 0;
	// Enviar caracteres hasta encontrar el car�cter nulo
	while (text[i] != '\0') {
		while (!(UCSR0A & (1<<UDRE0)));  // Esperar a que el buffer est� vac�o
		UDR0 = text[i];  // Enviar el car�cter
		i++;
	}
}
