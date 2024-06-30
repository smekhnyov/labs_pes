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
  Serial.begin(9600);
}

void enter_password(int password)
{
  char key;
  int try_count = 0;
  int try_password = 0;
  bool enter_flag = false;
  while (!enter_flag)
  {
    key = keypad.getKey();
    if (key != NO_KEY)
    {
      if (key != '#' && key != '*')
      {
        lcd.print(key);
        if (try_password == 0)
        {
          try_password = key - '0';
        }
        else
        {
          try_password = try_password*10 + (key - '0');
        }
      }
      else if (key == '#')
      {
        try_count++;
        Serial.print(try_password);
        delay(500);
        if (Serial.available() > 0)
        {
          char answer = Serial.read();
          if (answer == 'D')
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Correct!");
            enter_flag = true;
          }
          else if (answer == 'E')
          {
            try_password = 0;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Incorrect!");
            lcd.setCursor(0,1);
            lcd.print("Popitok: ");
            lcd.print(3 - try_count);
            delay(1000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(password);
            lcd.setCursor(0,1);
          }
        }
      }
      else if (key == '*')
      {
        try_password = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(password);
        lcd.setCursor(0,1);
      }
    }
    if (try_count > 2)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Popitok nema!");
      enter_flag = true;
    }
  }
}

void receive_password()
{
  bool received_responce = false;
  while (!received_responce)
  {
    if (Serial.available() > 0)
    {
      int password = Serial.parseInt();
      received_responce = true;
      Serial.print('C');
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(password);
      lcd.setCursor(0,1);
      enter_password(password);
    }
  }
}

void loop() {
  if (Serial.available() > 0) 
  {  
    char command = Serial.read();
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
