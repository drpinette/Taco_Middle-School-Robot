//#define DEBUG
#include <Taco.h>
#include <Condition.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>


RobotController* RC;


void setup() {
  pinMode(13, OUTPUT);
  // put your setup code here, to run once:
 while(digitalRead(START) == LOW)
 {
 }
 digitalWrite(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
