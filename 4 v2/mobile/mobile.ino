#include <LiquidCrystal.h>
#include <Keypad.h>

#define LCD_RS 8
#define LCD_En 7
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 21
#define LCD_D7 20

#define KEY_R1 17
#define KEY_R2 16
#define KEY_R3 15
#define KEY_R4 14
#define KEY_C1 12
#define KEY_C2 11
#define KEY_C3 10

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {KEY_R1, KEY_R2, KEY_R3, KEY_R4};
byte colPins[COLS] = {KEY_C1, KEY_C2, KEY_C3};

LiquidCrystal lcd(LCD_RS, LCD_En, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  lcd.begin(16,2);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    lcd.print(key);
  }
}
