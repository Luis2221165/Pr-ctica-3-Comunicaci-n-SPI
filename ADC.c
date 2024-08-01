#include "ADC.h"

 void lcd_start()
 {
	PORTB &=~ (1<<PORTB0);
	_delay_us(1);
	PORTB |=  (1<<PORTB0);
 }

 void lcd_write(uint8_t type, uint8_t inst_data)
 {
	if (type)
		PORTB |=  (1<<PORTB1);
	else
		PORTB &=~ (1<<PORTB1);

	PORTD = (PIND & 0x0F) | (inst_data & 0xF0);
	lcd_start();
	PORTD = (PIND & 0x0F) | ((inst_data<<4) & 0xF0);
	lcd_start();
	_delay_ms(2);
 }

 void lcd_init()
 {
	DDRB  |=  (1<<DDB0) | (1<<DDB1);	
	DDRD  |=  0xF0;
	
	PORTB &=~ ((1<<PORTB0) | (1<<PORTB0));
	
	PORTD  = (PIND & 0x0F) | 0x30;

	_delay_ms(100);
	lcd_start();
	_delay_ms(5);
	lcd_start();
	_delay_us(100);
	lcd_start();
	_delay_us(100);

	PORTD = (PIND & 0x0F) | 0x20;
	
	lcd_start();
	_delay_us(100);

	lcd_write(LCD_INST, 0x2C);
	lcd_write(LCD_INST, 0x08);
	lcd_write(LCD_INST, 0x01);
	lcd_write(LCD_INST, 0x06);	
	
	lcd_write(LCD_INST, 0x0C);
 }

void lcd_print(char *s)
{
	while(*s)
	{
		lcd_write(LCD_DATA, *s);
		s++;
	}
}