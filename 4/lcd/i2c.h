#include <avr/io.h>
#include <util/twi.h>

#define F_CPU 16000000UL // Частота микроконтроллера
#define SCL_CLOCK 100000L // Частота шины I2C (100 кГц)

void i2c_init(void) {
    TWSR = 0x00; // Установить делитель частоты на 1
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2; // Установить частоту I2C
    TWCR = (1 << TWEN); // Включить TWI
}

void i2c_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Отправить START
    while (!(TWCR & (1 << TWINT))); // Ожидать завершения
}

void i2c_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Отправить STOP
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN); // Отправить данные
    while (!(TWCR & (1 << TWINT))); // Ожидать завершения
}
