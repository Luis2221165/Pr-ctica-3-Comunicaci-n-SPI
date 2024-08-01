//***************************************************************************
//Universidad del valle de Guatemala
//IE2023: Programaci?n de Microcontroladores
//Autor: Luis Angel Ramirez Or?zco
//Proyecto: Práctica 3
//Hardware: ATMEGA328P
//***************************************************************************


#include <avr/io.h>
#include <stdint.h>
#include "UART.h"
#include "SPI.h"

const uint8_t ledPins[] = {PD2, PD3, PD4, PD5, PD6, PD7, PB0, PB1};

void initLEDs() {
	// Configura los pines de los LEDs como salida
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRB |= (1 << DDB0) | (1 << DDB1);
}

void showValueOnLeds(uint8_t value) {
	for (uint8_t i = 0; i < 8; i++) {
		if (value & (1 << i)) {
			PORTD &= ~(1 << ledPins[i]); // Enciende el LED (si está conectado a negativo)
			} else {
			PORTD |= (1 << ledPins[i]); // Apaga el LED
		}
	}
}

void setup() {
	initUART9600();     // Inicia comunicación con la computadora
	InitSPI(SPI_MASTER_OSC_DIV16, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE); // Inicia comunicación con el esclavo
	initLEDs();
}

void loop() {
	writeTextUART("¿Que deseas?\n");
	writeTextUART("1. Leer\n");
	writeTextUART("2. Enviar\n");

	while (!(UCSR0A & (1 << RXC0))) {
		// Espera una respuesta de la computadora
	}

	char response = UDR0;

	if (response == '1') {
		writeTextUART("¿Quieres ver valor?\n");

		while (!(UCSR0A & (1 << RXC0))) {
			// Espera una respuesta de la computadora
		}

		char viewValue = UDR0;

		if (viewValue == 's' || viewValue == 'S') {
			spiWrite(0x01); // Solicita los valores al esclavo

			while (!spiDataReady()) {
				// Espera los valores del esclavo
			}

			uint8_t val1 = spiRead();
			uint8_t val2 = spiRead();

			char buffer[50];
			snprintf(buffer, sizeof(buffer), "Valor de potenciometro 1: %u\n", val1);
			writeTextUART(buffer);
			snprintf(buffer, sizeof(buffer), "Valor de potenciometro 2: %u\n", val2);
			writeTextUART(buffer);
		}
		} else if (response == '2') {
		writeTextUART("Envia un valor entre 0 y 255:\n");

		while (!(UCSR0A & (1 << RXC0))) {
			// Espera un valor de la computadora
		}

		uint8_t value = UDR0 - '0'; // Convierte el caracter a un número

		if (value >= 0 && value <= 255) {
			showValueOnLeds(value);
			} else {
			writeTextUART("Valor fuera de rango. Intenta de nuevo.\n");
		}
	}

	_delay_ms(1000); // Pequeña pausa antes de volver a mostrar el menú
}

int main() {
	setup();
	while (1) {
		loop();
	}
}
