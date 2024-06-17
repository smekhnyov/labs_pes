#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lcd/lcd.h"

void init()
{
    DDR_LCD_SDA |= (1<<LCD_SDA);
    PORT_LCD_SDA &= ~(1<<LCD_SDA);

    DDR_LCD_SCL |= (1<<LCD_SCL);
    PORT_LCD_SCL &= ~(1<<LCD_SCL);

    DDR_KEY_R1 &= ~(1<<KEY_R1);
    PORT_KEY_R1 |= (1<<KEY_R1);

    DDR_KEY_R2 &= ~(1<<KEY_R2);
    PORT_KEY_R2 |= (1<<KEY_R2);

    DDR_KEY_R3 &= ~(1<<KEY_R3);
    PORT_KEY_R3 |= (1<<KEY_R3);

    DDR_KEY_R4 &= ~(1<<KEY_R4);
    PORT_KEY_R4 |= (1<<KEY_R4);

    DDR_KEY_C1 &= ~(1<<KEY_C1);
    PORT_KEY_C1 |= (1<<KEY_C1);

    DDR_KEY_C2 &= ~(1<<KEY_C2);
    PORT_KEY_C2 |= (1<<KEY_C2);

    DDR_KEY_C3 &= ~(1<<KEY_C3);
    PORT_KEY_C3 |= (1<<KEY_C3);

    UBRRL=round(F_CPU/(16*9600-1.0));//103
	UCSRB=(1<<RXEN)|(1<<RXCIE);
	UCSRC=(1<<URSEL)|(3<<UCSZ0);

    sei();
}

int main()
{
    init();

    i2c_init();
    lcd_init();
    lcd_set_cursor(0, 0);
    lcd_print("Hello, World!");
    while (1)
    {
        // Ваш основной код
    }
}