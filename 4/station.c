#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

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
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	UCSRC=(1<<URSEL)|(3<<UCSZ0);

    sei(); // Enable global interrupts
}

int main()
{
    init();

    PORT_LED_1_R |= (1<<LED_1_R);
    PORT_LED_1_Y |= (1<<LED_1_Y);
    PORT_LED_1_G |= (1<<LED_1_G);
    PORT_LED_2_R |= (1<<LED_2_R);
    PORT_LED_2_Y |= (1<<LED_2_Y);
    PORT_LED_2_G |= (1<<LED_2_G);
    PORT_LED_3_R |= (1<<LED_3_R);
    PORT_LED_3_Y |= (1<<LED_3_Y);
    PORT_LED_3_G |= (1<<LED_3_G);
}