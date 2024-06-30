#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
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
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[ROWS] = {KEY_R1, KEY_R2, KEY_R3, KEY_R4};
byte colPins[COLS] = {KEY_C1, KEY_C2, KEY_C3};

LiquidCrystal lcd(LCD_RS, LCD_En, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
}

int waitInt()
{
  while (Serial.available() == 0)
  {
  }
  int receivedData = Serial.parseInt();
  return receivedData;
}

char waitChar()
{
  while (Serial.available() == 0)
  {
  }
  char receivedData = Serial.read();
  return receivedData;
}

void enterPassword(int correctPassword) {
  int enteredPassword = 0;
  int incorrectAttempts = 0;
  bool isCorrect = false;

  while (!isCorrect) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      if (key != '#' && key != '*') {
        lcd.print('*');
        enteredPassword = enteredPassword * 10 + (key - '0');
      } else if (key == '#') {
        incorrectAttempts++;
        Serial.print(enteredPassword);
        delay(100);
        char response = waitChar();
        if (response == 'D') {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Correct!");
          isCorrect = true;
        } else if (response == 'E') {
          enteredPassword = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Incorrect!");
          lcd.setCursor(0, 1);
          lcd.print("Popitok: ");
          lcd.print(3 - incorrectAttempts);
          delay(1000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(correctPassword);
          lcd.setCursor(0, 1);
        }
      } else if (key == '*') {
        enteredPassword = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(correctPassword);
        lcd.setCursor(0, 1);
      }
    }

    if (incorrectAttempts > 2 && !isCorrect) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Popitok nema!");
      isCorrect = true;
    }
  }
}

void receive_password()
{
  char answer = waitChar();
  if (answer == 'T')
  {
    Serial.print('O');
    int password = waitInt();
    Serial.print('C');
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(password);
    lcd.setCursor(0, 1);
    enterPassword(password);
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    char command = waitChar();
    switch (command)
    {
    case 'A':
      Serial.print('B');
      lcd.print("Initialize...");
      delay(500);
      receive_password();      
    }
  }
}
