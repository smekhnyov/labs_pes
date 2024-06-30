#include <GyverStepper.h>
#include <SoftwareSerial.h>
#include <time.h>

#define LED_1_R 15
#define LED_1_Y 14
#define LED_1_G 22
#define LED_2_R 2
#define LED_2_Y 3
#define LED_2_G 4
#define LED_3_R 5
#define LED_3_Y 20
#define LED_3_G 21

#define ENCODER_1_DT 13
#define ENCODER_1_CLK 12
#define ENCODER_2_DT 11
#define ENCODER_2_CLK 10

#define STEPPER_PA 19
#define STEPPER_PB 18
#define STEPPER_NA 17
#define STEPPER_NB 16

#define TUMBLER_1 6
#define TUMBLER_2 7
#define TUMBLER_3 8

#define SPEAKER 9

bool tumbler_1_flag = false;
bool tumbler_2_flag = false;
bool tumbler_3_flag = false;

GStepper<STEPPER2WIRE> stepper(32, STEPPER_NB, STEPPER_PB, STEPPER_NA, STEPPER_PA);

void setup() {
  pinMode(TUMBLER_1, INPUT);
  pinMode(TUMBLER_2, INPUT);
  pinMode(TUMBLER_3, INPUT);

  pinMode(SPEAKER, OUTPUT);

  pinMode(ENCODER_1_DT, INPUT_PULLUP);
  pinMode(ENCODER_1_CLK, INPUT_PULLUP);
  pinMode(ENCODER_2_DT, INPUT_PULLUP);
  pinMode(ENCODER_2_CLK, INPUT_PULLUP);

  stepper.setSpeed(300);

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

int generate_random_key() {
  return rand()%9999;
}

void enter_key() {
  int key = generate_random_key();
  bool flag = false;
  Serial.print(key);
  while (!flag) {
    if (Serial.available() > 0) 
    {  
      char command = Serial.read();
      if (command == 'C') {
        bool flag2 = false;
        while (!flag2) {
          if (Serial.available() > 0)
          {
            int password = Serial.parseInt();
            if (password == key) {
              Serial.print('D');
              flag = true;
              flag2 = true;
              digitalWrite(LED_1_Y, LOW);
              digitalWrite(LED_1_G, HIGH);
            }
            else {
              Serial.print('E');
              digitalWrite(SPEAKER, HIGH);
              delay(100);
              digitalWrite(SPEAKER, LOW);
            }
          }
        }
      }
    }
  }
}

void mobile_connect() {
  bool flag = false;
  while (!flag) {
    if (Serial.available() > 0) 
    {  
      char command = Serial.read();
      if (command == 'B') {
        digitalWrite(LED_1_R, LOW);
        digitalWrite(LED_1_Y, HIGH);
        enter_key();
        flag = true;
      }
    }
  }
}
void wait_1st_tumbler() {
  while (!digitalRead(TUMBLER_1)) {
    tumbler_1_flag = true;
    Serial.print('A');
    mobile_connect();
  }
}

void loop() {
  wait_1st_tumbler();
  
  // stepper.runSpeed();
}
