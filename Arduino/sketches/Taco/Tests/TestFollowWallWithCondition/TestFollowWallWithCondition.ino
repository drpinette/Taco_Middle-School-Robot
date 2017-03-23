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
  
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Left, CW), LessThan, 24.0, RC));
  RC->stop();
 
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

} 
