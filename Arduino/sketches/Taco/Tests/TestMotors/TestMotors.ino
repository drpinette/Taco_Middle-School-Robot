#include <Taco.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  RC->go(North, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(5000);
  RC->stop();
}

void loop() {
  // put your main code here, to run repeatedly:

} 
