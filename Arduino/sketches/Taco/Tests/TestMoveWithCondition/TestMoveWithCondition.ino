#define DEBUG
#include <Taco.h>
#include <Condition.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  delay(5000);
  Serial.begin(9600);
  _DS("Starting"); _NL;
  RC->move(North, DEFAULT_SPEED, new False());
  RC->stop();
 
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

} 
