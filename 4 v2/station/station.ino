#include <GyverStepper.h>

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

GStepper<STEPPER2WIRE> stepper(32, STEPPER_NB, STEPPER_PB, STEPPER_NA, STEPPER_PA);


void setup() {
  // pinMode(LED_1_R, OUTPUT);
  // pinMode(LED_1_Y, OUTPUT);
  // pinMode(LED_1_G, OUTPUT);
  // pinMode(LED_2_R, OUTPUT);
  // pinMode(LED_2_Y, OUTPUT);
  // pinMode(LED_2_G, OUTPUT);
  // pinMode(LED_3_R, OUTPUT);
  // pinMode(LED_3_Y, OUTPUT);
  // pinMode(LED_3_G, OUTPUT);

  stepper.setRunMode(FOLLOW_POS);
  stepper.setSpeed(10);
  stepper.setTarget(-32);
}

void loop() {
  // digitalWrite(LED_1_R, HIGH);
  // digitalWrite(LED_1_Y, HIGH);
  // digitalWrite(LED_1_G, HIGH);
  // digitalWrite(LED_2_R, HIGH);
  // digitalWrite(LED_2_Y, HIGH);
  // digitalWrite(LED_2_G, HIGH);
  // digitalWrite(LED_3_R, HIGH);
  // digitalWrite(LED_3_Y, HIGH);
  // digitalWrite(LED_3_G, HIGH);
  if (!stepper.tick()) {
    static bool dir;
    dir = !dir;
    stepper.setTarget(dir ? -1024 : 1024);
  }
}
