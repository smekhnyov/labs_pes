#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "lib/UART.h"

bool tumbler_1_flag = false;
bool tumbler_2_flag = false;
bool tumbler_3_flag = false;

void init()
{
    // STEPPER
    DDR_STEPPER_PA |= (1<<STEPPER_PA); // выход
    PORT_STEPPER_PA &= ~(1<<STEPPER_PA); // выключен

    DDR_STEPPER_PB |= (1<<STEPPER_PB); // выход
    PORT_STEPPER_PB &= ~(1<<STEPPER_PB); // выключен

    DDR_STEPPER_NA |= (1<<STEPPER_NA); // выход
    PORT_STEPPER_NA &= ~(1<<STEPPER_NA); // выключен

    DDR_STEPPER_NB |= (1<<STEPPER_NB); // выход
    PORT_STEPPER_NB &= ~(1<<STEPPER_NB); // выключен

    // ENCODER 1
    DDR_ENCODER_1_DT &= ~(1<<ENCODER_1_DT); // вход
    PORT_ENCODER_1_DT |= (1<<ENCODER_1_DT); // выключен
    DDR_ENCODER_1_CLK &= ~(1<<ENCODER_1_CLK); // вход
    PORT_ENCODER_1_CLK |= (1<<ENCODER_1_CLK); // выключен

    // ENCODER 2
    DDR_ENCODER_2_DT &= ~(1<<ENCODER_2_DT); // вход
    PORT_ENCODER_2_DT |= (1<<ENCODER_2_DT); // выключен
    DDR_ENCODER_2_CLK &= ~(1<<ENCODER_2_CLK); // вход
    PORT_ENCODER_2_CLK |= (1<<ENCODER_2_CLK); // выключен

    // SPEAKER
    DDR_SPEAKER |= (1<<SPEAKER); // выход
    PORT_SPEAKER &= ~(1<<SPEAKER); // выключен

    // TUMBLER 1
    DDR_TUMBLER_1 &= ~(1<<TUMBLER_1); // вход
    PORT_TUMBLER_1 |= (1<<TUMBLER_1); // выключен

    // TUMBLER 2
    DDR_TUMBLER_2 &= ~(1<<TUMBLER_2); // вход
    PORT_TUMBLER_2 |= (1<<TUMBLER_2); // выключен

    // TUMBLER 3
    DDR_TUMBLER_3 &= ~(1<<TUMBLER_3); // вход
    PORT_TUMBLER_3 |= (1<<TUMBLER_3); // выключен

    // LED 1 R
    DDR_LED_1_R |= (1<<LED_1_R); // выход
    PORT_LED_1_R &= ~(1<<LED_1_R); // выключен

    // LED 1 Y
    DDR_LED_1_Y |= (1<<LED_1_Y); // выход
    PORT_LED_1_Y &= ~(1<<LED_1_Y); // выключен

    // LED 1 G
    DDR_LED_1_G |= (1<<LED_1_G); // выход
    PORT_LED_1_G &= ~(1<<LED_1_G); // выключен

    // LED 2 R
    DDR_LED_2_R |= (1<<LED_2_R); // выход
    PORT_LED_2_R &= ~(1<<LED_2_R); // выключен

    // LED 2 Y
    DDR_LED_2_Y |= (1<<LED_2_Y); // выход
    PORT_LED_2_Y &= ~(1<<LED_2_Y); // выключен

    // LED 2 G
    DDR_LED_2_G |= (1<<LED_2_G); // выход
    PORT_LED_2_G &= ~(1<<LED_2_G); // выключен

    // LED 3 R
    DDR_LED_3_R |= (1<<LED_3_R); // выход
    PORT_LED_3_R &= ~(1<<LED_3_R); // выключен

    // LED 3 Y
    DDR_LED_3_Y |= (1<<LED_3_Y); // выход
    PORT_LED_3_Y &= ~(1<<LED_3_Y); // выключен

    // LED 3 G
    DDR_LED_3_G |= (1<<LED_3_G); // выход
    PORT_LED_3_G &= ~(1<<LED_3_G); // выключен

    // UART
    UBRRL=round(F_CPU/(16*9600-1.0)); //103
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);

    sei(); // Enable global interrupts
}

unsigned char* generate_random_key()
{
    srand(TCNT0);
    static unsigned char number_str[5];
    for (int i = 0; i < 4; i++)
    {
        number_str[i] = '0' + rand()%10;
    }
    number_str[4] = '\0';
    return number_str;
}

void enter_key() {
    unsigned char* key = generate_random_key();
    unsigned char response;
    unsigned char responce_accept;
    bool received_responce = false;
    while (!received_responce) {
        uart_transmit_string(key);
        response = uart_receive();
        if (response == 'C')
        {
            bool received_accept = false;
            while (!received_accept)
            {
                responce_accept = uart_receive();
                if (responce_accept == 'D')
                {
                    received_accept = true;
                    received_responce = true;
                    PORT_LED_1_Y &= ~(1<<LED_1_Y);
                    PORT_LED_1_G |= (1<<LED_1_G);
                }
            }
        }
    }
}

void mobile_connect() {
    unsigned char request = 'A';
    unsigned char response;
    bool received_responce = false;
    while (!received_responce) {
        uart_transmit(request);
        _delay_ms(100);

        response = uart_receive();
        if (response == 'B') {
            received_responce = true;
        }
    }
    PORT_LED_1_R &= ~(1<<LED_1_R);
    PORT_LED_1_Y |= (1<<LED_1_Y);
    enter_key();
}

void wait_1st_tumbler() {
    while (true)
    {
        if (!(PIN_TUMBLER_1 & (1 << TUMBLER_1)))
        {
            tumbler_1_flag = true;
            mobile_connect();
        }
    }
}

int main()
{
    init();

    PORT_LED_1_R |= (1<<LED_1_R);
    PORT_LED_2_R |= (1<<LED_2_R);
    PORT_LED_3_R |= (1<<LED_3_R);

    wait_1st_tumbler();
}
