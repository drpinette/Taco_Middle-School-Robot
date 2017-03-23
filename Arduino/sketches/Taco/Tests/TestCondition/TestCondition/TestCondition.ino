#define DEGUG
#include <Taco.h>
#include <Condition.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>


RobotController* RC;
Condition* condition;

void setup() {
  pinMode(13, OUTPUT);
  RC = new RobotController();
  RC->initialize();
  Serial.begin(9600);
  condition = new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 24.0, RC);
  digitalWrite(13, LOW);
}

void loop() {
  bool testResult = condition->test();
  digitalWrite(13, testResult);
  delay(10);
}
