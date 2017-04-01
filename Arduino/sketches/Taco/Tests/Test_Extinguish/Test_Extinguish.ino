#define DEBUG
#include <Servo.h>
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
  RC->extinguish(true);
  RC->extinguish(false);
}

void loop() {
  // put your main code here, to run repeatedly:

} 
