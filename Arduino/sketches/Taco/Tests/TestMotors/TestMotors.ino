#define DEBUG
#include <Taco.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  delay(5000);
  Serial.begin(9600);
  /*
  RC->go(North, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(2000);
  RC->stop();
  RC->go(North, 0, Right, DEFAULT_SPEED, NoRotation, 0);
  delay(2000);
  RC->stop();
  */
  RC->go(South, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(2000);
  RC->stop();
  /*
  RC->go(North, 0, Left, DEFAULT_SPEED, NoRotation, 0);
  delay(2000);
  RC->stop();
  RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
  delay(2000);
  RC->stop();
  RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
  delay(2000);
  RC->stop();
  */
}

void loop() {
  // put your main code here, to run repeatedly:

} 
