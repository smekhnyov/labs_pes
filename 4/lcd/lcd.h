#include <util/delay.h>
#include "i2c.h" // Ваши функции i2c

#define LCD_ADDRESS 0x27 // Адрес вашего I2C экрана

void lcd_command(uint8_t cmd) {
    uint8_t data_u = (cmd & 0xF0);
    uint8_t data_l = ((cmd << 4) & 0xF0);
    i2c_start();
    i2c_write(LCD_ADDRESS << 1);
    i2c_write(data_u | 0x0C); // RS = 0, E = 1
    i2c_write(data_u | 0x08); // RS = 0, E = 0
    i2c_write(data_l | 0x0C); // RS = 0, E = 1
    i2c_write(data_l | 0x08); // RS = 0, E = 0
    i2c_stop();
}

void lcd_data(uint8_t data) {
    uint8_t data_u = (data & 0xF0);
    uint8_t data_l = ((data << 4) & 0xF0);
    i2c_start();
    i2c_write(LCD_ADDRESS << 1);
    i2c_write(data_u | 0x0D); // RS = 1, E = 1
    i2c_write(data_u | 0x09); // RS = 1, E = 0
    i2c_write(data_l | 0x0D); // RS = 1, E = 1
    i2c_write(data_l | 0x09); // RS = 1, E = 0
    i2c_stop();
}

void lcd_init(void) {
    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
    _delay_ms(200);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 + col : 0xC0 + col;
    lcd_command(address);
}

void lcd_print(char* str) {
    while (*str) {
        lcd_data(*str++);
    }
}
