#include <GyverStepper.h>
#include <time.h>
#include <GyverEncoder.h>

#define LED_1_R 18
#define LED_1_Y 17
#define LED_1_G 16
#define LED_2_R 15
#define LED_2_Y 14
#define LED_2_G 13
#define LED_3_R 12
#define LED_3_Y 11
#define LED_3_G 10

#define ENCODER_1_DT 4
#define ENCODER_1_CLK 20
#define ENCODER_2_DT 5
#define ENCODER_2_CLK 6

#define STEPPER_PA 19
#define STEPPER_PB 22
#define STEPPER_NA 2
#define STEPPER_NB 3

#define TUMBLER_1 6
#define TUMBLER_2 7
#define TUMBLER_3 8

#define SPEAKER 9

bool tumbler_1_flag = false;
bool tumbler_2_flag = false;
bool tumbler_3_flag = false;

GStepper<STEPPER4WIRE> stepper(100, STEPPER_NB, STEPPER_PB, STEPPER_NA, STEPPER_PA);
Encoder enc1(ENCODER_1_CLK, ENCODER_1_DT);
Encoder enc2(ENCODER_2_CLK, ENCODER_2_DT);

void setup()
{
  pinMode(TUMBLER_1, INPUT_PULLUP);
  pinMode(TUMBLER_2, INPUT_PULLUP);
  pinMode(TUMBLER_3, INPUT_PULLUP);

  pinMode(SPEAKER, OUTPUT);

  enc1.setType(TYPE1);
  enc1.setDirection(REVERSE);
  enc2.setType(TYPE1);
  enc2.setDirection(REVERSE);

  stepper.setSpeed(360);

  pinMode(LED_1_R, OUTPUT);
  pinMode(LED_1_Y, OUTPUT);
  pinMode(LED_1_G, OUTPUT);
  pinMode(LED_2_R, OUTPUT);
  pinMode(LED_2_Y, OUTPUT);
  pinMode(LED_2_G, OUTPUT);
  pinMode(LED_3_R, OUTPUT);
  pinMode(LED_3_Y, OUTPUT);
  pinMode(LED_3_G, OUTPUT);

  Serial.begin(9600);

  digitalWrite(LED_1_R, HIGH);
  digitalWrite(LED_2_R, HIGH);
  digitalWrite(LED_3_R, HIGH);

  srand(time(NULL));
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

int generate_random_key()
{
  return rand() % 9999;
}

void enter_key()
{
  int key = 1234;
  Serial.print('T');
  char answer = waitChar();
  if (answer == 'O')
  {
    Serial.print(key);
  }

  char pwd_transmitted = waitChar();
  if (pwd_transmitted == 'C')
  {
    bool flag2 = false;
    while (!flag2)
    {
      int password = waitInt();
      if (password == key)
      {
        Serial.print('D');
        flag2 = true;
        digitalWrite(LED_1_Y, LOW);
        digitalWrite(LED_1_G, HIGH);
      }
      else
      {
        Serial.print('E');
        digitalWrite(SPEAKER, HIGH);
        delay(100);
        digitalWrite(SPEAKER, LOW);
      }
    }
  }
}

void mobile_connect()
{
  char command = waitChar();
  if (command == 'B')
  {
    digitalWrite(LED_1_R, LOW);
    digitalWrite(LED_1_Y, HIGH);
    enter_key();
  }
}
void wait_1st_tumbler()
{
  if (tumbler_1_flag)
  {
    Serial.print('A');
    mobile_connect();
  }
}

long stepper_position = 0;

void stepper_control()
{
  char answer = waitChar();
  if (answer == 'S')
  {
    int freq = 0;
    while (tumbler_2_flag && !tumbler_3_flag)
    {
      enc1.tick();
      if (enc1.isRight()) {
        stepper_position++;
        stepper.setTarget(stepper_position);
        Serial.print(stepper_position);
      }
      if (enc1.isLeft()) {
        stepper_position--;
        stepper.setTarget(stepper_position);
        Serial.print(stepper_position);
      }
      stepper.tick();
  
      enc2.tick();
      if (enc2.isRight()) {
        freq++;
      }
      if (enc2.isLeft()) {
        freq--;
      }

      tumbler_3_flag = !digitalRead(TUMBLER_3);
    }
  }
}

void wait_2nd_tumbler()
{
  if (tumbler_2_flag)
  {
    Serial.print('T');
    delay(100);
    Serial.print(stepper.getCurrentDeg());
    stepper_control();
  }
}

void loop()
{
  if (!digitalRead(TUMBLER_1) && !tumbler_1_flag && !tumbler_2_flag && !tumbler_3_flag)
  {
    tumbler_1_flag = true;
    wait_1st_tumbler();
  }
  if (!digitalRead(TUMBLER_2) && tumbler_1_flag && !tumbler_2_flag && !tumbler_3_flag)
  {
    tumbler_2_flag = true;
    wait_2nd_tumbler();
  }
  // stepper.runSpeed();
}