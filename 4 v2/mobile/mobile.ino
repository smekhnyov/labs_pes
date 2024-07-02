#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <EEPROM.h>

#define LCD_RS PIN_PB0
#define LCD_En PIN_PD7
#define LCD_D4 PIN_PD6
#define LCD_D5 PIN_PD5
#define LCD_D6 PIN_PB7
#define LCD_D7 PIN_PB6

#define KEY_R1 PIN_PC3
#define KEY_R2 PIN_PC2
#define KEY_R3 PIN_PC1
#define KEY_R4 PIN_PC0
#define KEY_C1 PIN_PB4
#define KEY_C2 PIN_PB3
#define KEY_C3 PIN_PB2

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

float waitFloat()
{
  while (Serial.available() == 0)
  {
  }
  float receivedData = Serial.parseFloat();
  return receivedData;
}

void enterPassword(int correctPassword)
{
  int enteredPassword = 0;
  int incorrectAttempts = 0;
  bool isCorrect = false;

  while (!isCorrect)
  {
    char key = keypad.getKey();

    if (key != NO_KEY)
    {
      if (key != '#' && key != '*')
      {
        lcd.print('*');
        enteredPassword = enteredPassword * 10 + (key - '0');
      }
      else if (key == '#')
      {
        incorrectAttempts++;
        Serial.print(enteredPassword);
        delay(100);
        char response = waitChar();
        if (response == 'D')
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Correct!");
          isCorrect = true;
        }
        else if (response == 'E')
        {
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
      }
      else if (key == '*')
      {
        enteredPassword = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(correctPassword);
        lcd.setCursor(0, 1);
      }
    }

    if (incorrectAttempts > 2 && !isCorrect)
    {
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

void get_settings()
{
  int progress;
  char prefix = waitChar();
  while (prefix != 'X')
  {
    progress = waitInt();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Progress: ");
    lcd.print(progress);
    lcd.print("%");
    prefix = waitChar();
  }
  lcd.clear();
}

void display_progress_third_switch()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Progress: 0%");

  int num_packets = waitInt();
  Serial.print('G');
  int received_packets = 0;
  int progress = 0;
  bool abort_flag = false;

  while (received_packets < num_packets && !abort_flag)
  {
    char prefix = waitChar();
    if (prefix == 'A')
    {
      int received_data = waitInt();
      if (received_data > 0)
      {
        received_packets++;
        progress = (received_packets * 100) / num_packets;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Progress: ");
        lcd.print(progress);
        lcd.print("%");
        delay(100);
      }
    }
    else if (prefix == 'B')
    {
      continue;
    }
    else if (prefix == 'E')
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error!");
      abort_flag = true;
      delay(1000);
    } 
  }
  EEPROM.put(0, received_packets);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Received:");
  lcd.setCursor(0, 1);
  lcd.print(received_packets);
  lcd.print("/");
  lcd.print(num_packets);
}

void loop()
{
  char command = waitChar();
  switch (command)
  {
  case 'A':
    Serial.print('B');
    lcd.print("Initialize...");
    delay(500);
    receive_password();
  case 'T':
    Serial.print('S');
    get_settings();
  case 'M':
    Serial.print('N');
    display_progress_third_switch();
  }
}
