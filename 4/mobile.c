#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lib/matrix_keypad.h"
#include "lib/lcd.h"
#include "lib/UART.h"

const uint8_t row_pins[4] = {3, 2, 1, 0};
const uint8_t col_pins[3] = {4, 3, 2};


void init()
{
    // DDR_LCD_SDA |= (1<<LCD_SDA);
    // PORT_LCD_SDA &= ~(1<<LCD_SDA);

    // DDR_LCD_SCL |= (1<<LCD_SCL);
    // PORT_LCD_SCL &= ~(1<<LCD_SCL);

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
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC=(1<<URSEL)|(3<<UCSZ0)|(1<<UCSZ1);

    sei();
}

void wait_for_connect()
{
    unsigned char request;
    unsigned char response = 'B';
    bool received_request = false;
    while (!received_request) {
        request = uart_receive();
        //lcd_char(request);

        if (request == 'A') {
            received_request = true;
            uart_transmit(response);
            lcd_string("Initialize...");
        }
    }
}

void enter_key()
{
    char key;
    char* request;
    bool received_responce = false;
    while (!received_responce) {
        request = uart_receive_number();
        uart_transmit('C');
        lcd_clear();
        lcd_string(request);
        // key = keypad_get_key(row_pins, col_pins);
        // if (key)
        // {
        //     lcd_char(key);
        // }
    }
}

int main()
{
    init();
    lcd_init(); // Инициализация LCD
    keypad_init(row_pins, col_pins); 
    wait_for_connect();
    enter_key();
    return 0;
}
