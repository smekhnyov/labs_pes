#include <GyverStepper.h>
#include <time.h>
#include <GyverEncoder.h>

#define LED_1_R PIN_PC4
#define LED_1_Y PIN_PC3
#define LED_1_G PIN_PC2
#define LED_2_R PIN_PC1
#define LED_2_Y PIN_PC0
#define LED_2_G PIN_PB5
#define LED_3_R PIN_PB4
#define LED_3_Y PIN_PB3
#define LED_3_G PIN_PB2

#define ENCODER_1_DT PIN_PB6
#define ENCODER_1_CLK PIN_PD4
#define ENCODER_2_DT PIN_PD5
#define ENCODER_2_CLK PIN_PB7

#define STEPPER_PA PIN_PC5
#define STEPPER_PB PIN_PC6
#define STEPPER_NA PIN_PD2
#define STEPPER_NB PIN_PD3

#define TUMBLER_1 PIN_PD6
#define TUMBLER_2 PIN_PD7
#define TUMBLER_3 PIN_PB0

#define SPEAKER 9

bool tumbler_1_flag = false;
bool tumbler_2_flag = false;
bool tumbler_3_flag = false;

bool flag_done = false;

GStepper<STEPPER4WIRE> stepper(100, STEPPER_NB, STEPPER_PB, STEPPER_NA, STEPPER_PA);
Encoder enc1(ENCODER_1_CLK, ENCODER_1_DT);
Encoder enc2(ENCODER_2_CLK, ENCODER_2_DT);

unsigned long timer;

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
  timer = millis();
  randomSeed(timer);
  return random(1000, 10000);
}

void enter_key()
{
  int key = generate_random_key();
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
float freq = 110;
const float min_freq = 100;
const float max_freq = 120;
int motor_rate;
int freq_rate;
int total_rate;

int abc(int accuracy)
{
    if (accuracy <= 100) return accuracy;
    else if (accuracy > 100 && (accuracy / 100) % 2 == 1) return (((accuracy / 100) + 1)*100 - accuracy);
    else if (accuracy > 100 && (accuracy / 100) % 2 == 0) return (accuracy - (accuracy / 100)*100);
}

void stepper_control()
{
  int target = random(-100, 101);
  int zero_point;
  if (target > 0) zero_point = 50 - target;
  else zero_point = 50 + target;
  motor_rate = zero_point * 2;
  freq_rate = (freq-min_freq)/(max_freq-min_freq)*100;
  total_rate = (abc(abs(motor_rate)) + freq_rate)/2;
  int last_position = stepper_position;
  char answer = waitChar();
  if (answer == 'S')
  {
    Serial.print('P');
    Serial.print(total_rate);
    while (tumbler_2_flag && !tumbler_3_flag)
    {
      enc1.tick();
      if (enc1.isRight())
      {
        last_position = stepper_position;
        stepper_position++;
        if (target - stepper_position < target - last_position)
        {
          motor_rate = motor_rate + 2;
        }
        else
        {
          motor_rate = motor_rate - 2;
        }
        stepper.setTarget(stepper_position);
        total_rate = (abc(abs(motor_rate)) + freq_rate)/2;
        Serial.print('P');
        Serial.print(total_rate);
      }
      if (enc1.isLeft())
      {
        last_position = stepper_position;
        stepper_position--;
        if (target - stepper_position < target - last_position)
        {
          motor_rate = motor_rate + 2;
        }
        else
        {
          motor_rate = motor_rate - 2;
        }
        stepper.setTarget(stepper_position);
        total_rate = (abc(abs(motor_rate)) + freq_rate)/2;
        Serial.print('P');
        Serial.print(total_rate);
      }
      stepper.tick();

      enc2.tick();
      if (enc2.isRight() && freq < 120)
      {
        freq = freq + 0.1;
        freq_rate = (freq-min_freq)/(max_freq-min_freq)*100;
        total_rate = (abc(abs(motor_rate)) + freq_rate)/2;
        Serial.print('P');
        Serial.print(total_rate);
      }
      if (enc2.isLeft() && freq > 100)
      {
        freq = freq - 0.1;
        freq_rate = (freq-min_freq)/(max_freq-min_freq)*100;
        total_rate = (abc(abs(motor_rate)) + freq_rate)/2;
        Serial.print('P');
        Serial.print(total_rate);
      }

      
      if (total_rate > 80)
      {
        digitalWrite(LED_2_R, LOW);
        digitalWrite(LED_2_Y, HIGH);
      }
      else
      {
        digitalWrite(LED_2_R, HIGH);
        digitalWrite(LED_2_Y, LOW);
      }

      if (total_rate == 100)
      {
        digitalWrite(LED_2_Y, LOW);
        digitalWrite(LED_2_G, HIGH);
      }
      else
      {
        digitalWrite(LED_2_G, LOW);
      }

      if (!digitalRead(TUMBLER_3))
      {
        tumbler_3_flag = true;
        Serial.print('X');
      }
    }
  }
}

void wait_2nd_tumbler()
{
  if (tumbler_2_flag)
  {
    Serial.print('T');
    stepper_control();
  }
}

void send_packets()
{
  int lost_count = 0;
  bool abort_flag = false;
  digitalWrite(LED_3_R, LOW);
  digitalWrite(LED_3_Y, HIGH);
  char prefix = waitChar();
  if (prefix == 'N')
  {
    int num_packets = 255;     // Количество пакетов данных
    Serial.print(num_packets); // Отправка количества пакетов

    char prefix = waitChar();
    if (prefix == 'G')
    {
      for (int i = 1; i <= num_packets && !abort_flag; ++i)
      {
        int random_value = random(0, 101);
        if (random_value <= total_rate)
        {
          Serial.print('A');
          Serial.print(i); // Отправка номера пакета
          lost_count = 0;
          digitalWrite(SPEAKER, HIGH);
          delay(50);
          digitalWrite(SPEAKER, LOW);
        }
        else 
        {
          Serial.print('B');
          lost_count++;
          --i;
        }
        if (lost_count > 2)
        {
          Serial.print('E');
          abort_flag = true;
          digitalWrite(LED_3_Y, LOW);
          digitalWrite(SPEAKER, HIGH);
          for (int j = 0; j < 10; j++)
          {
             digitalWrite(LED_3_R, HIGH);
             delay(100);
             digitalWrite(LED_3_R, LOW);
             delay(100);
          }
          digitalWrite(LED_3_R, HIGH);
          digitalWrite(SPEAKER, LOW);
        }
        delay(100);      // Задержка для устойчивости
      }
      if (!abort_flag)
      {
        digitalWrite(LED_3_Y, LOW);
        digitalWrite(LED_3_G, HIGH);
      }
    }
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
  if (!digitalRead(TUMBLER_3) && tumbler_1_flag && tumbler_2_flag && !flag_done)
  {
    Serial.print('M');
    send_packets();
    flag_done = true;
  }
}
