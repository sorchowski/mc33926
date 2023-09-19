#include <MC33926.h>

//https://www.pololu.com/product/1213

#define IN1_PIN 6
#define IN2_PIN 9

MC33926 motorController(IN1_PIN, IN2_PIN, 12);

// 4095 is the maximum value for a 12-bit pwm signal on the Due

void setup() {
  motorController.setSpeed(100);
}

void loop() {
  delay(2000);
  motorController.setSpeed(200);
  delay(2000);
  motorController.setSpeed(0);
  delay(2000);
  motorController.setSpeed(-100);
  delay(2000);
  motorController.setSpeed(-200);
  delay(2000);
  motorController.setSpeed(0);
  delay(2000);
  motorController.setSpeed(100);

}