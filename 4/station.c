#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lib/UART.h"

bool tumbler_1_flag = false;
bool tumbler_2_flag = false;
bool tumbler_3_flag = false;

volatile int16_t encoder_position = 0;
volatile int16_t stepper_position = 0;

void init()
{
    srand(time(TCNT0));

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
    bool received_responce = false;
    while (!received_responce) {
        uart_transmit_string(key);
        response = uart_receive();
        if (response == 'C')
        {
            bool received_accept = false;
            while (!received_accept)
            {
                char* responce_password = uart_receive_number();
                if (strcmp(responce_password, key) == 0)
                {
                    uart_transmit('D');
                    received_accept = true;
                    received_responce = true;
                    PORT_LED_1_Y &= ~(1<<LED_1_Y);
                    PORT_LED_1_G |= (1<<LED_1_G);
                }
                else
                {
                    uart_transmit('E');
                    PORT_SPEAKER |= (1<<SPEAKER);
                    _delay_ms(100);
                    PORT_SPEAKER &= ~(1<<SPEAKER);
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

void stepper_step(int direction) {
    static uint8_t step = 0;
    const uint8_t step_sequence[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    // Направление вращения
    if (direction > 0) {
        step = (step + 1) % 4;
    } else if (direction < 0) {
        step = (step + 3) % 4;
    }

    // Выдача сигнала на шаговый драйвер
    if (step_sequence[step][0]) PORT_STEPPER_PA |= (1<<PIN_STEPPER_PA); else PORT_STEPPER_PA &= ~(1<<PIN_STEPPER_PA);
    if (step_sequence[step][1]) PORT_STEPPER_PB |= (1<<PIN_STEPPER_PB); else PORT_STEPPER_PB &= ~(1<<PIN_STEPPER_PB);
    if (step_sequence[step][2]) PORT_STEPPER_NA |= (1<<PIN_STEPPER_NA); else PORT_STEPPER_NA &= ~(1<<PIN_STEPPER_NA);
    if (step_sequence[step][3]) PORT_STEPPER_NB |= (1<<PIN_STEPPER_NB); else PORT_STEPPER_NB &= ~(1<<PIN_STEPPER_NB);
}

int generate_random_angle()
{
    int angle = rand()%360;
    return angle;
}

uint8_t read_gray_code_from_encoder() 
{ 
 uint8_t val=0; 

  if(!bit_is_clear(PIN_ENCODER_1_DT, ENCODER_1_DT)) 
	val |= (1<<1); 

  if(!bit_is_clear(PIN_ENCODER_1_CLK, ENCODER_1_CLK)) 
	val |= (1<<0); 

  return val; 
}

void set_settings() {
    int angle = generate_random_angle();
    uint8_t current_pos = 0, last_pos = 0;
    current_pos = read_gray_code_from_encoder(); 
    while (PIN_TUMBLER_3 & (1 << TUMBLER_3))
    {
        last_pos = read_gray_code_from_encoder();

        if(current_pos != last_pos) 
	    { 
		    if((current_pos==3 && last_pos==1) || (current_pos==0 && last_pos==2)) 
		    { 
                encoder_position++;
                uart_transmit('+');
		    } 
		    else if((current_pos==2 && last_pos==0) || (current_pos==1 && last_pos==3)) 
		    { 
                encoder_position--;
				uart_transmit('-');
		    } 

		   current_pos = last_pos; 
	    } 

	   _delay_ms(1); 

        // Приведение шагового двигателя в положение энкодера
        if (encoder_position > stepper_position) {
            stepper_step(1);  // Шаг вперед
            stepper_position++;
        } else if (encoder_position < stepper_position) {
            stepper_step(-1); // Шаг назад
            stepper_position--;
        }
    }
}

void wait_2nd_tumbler() {
    while (true)
    {
        if (!(PIN_TUMBLER_2 & (1 << TUMBLER_2)))
        {
            tumbler_2_flag = true;
            set_settings();
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
    wait_2nd_tumbler();
}
