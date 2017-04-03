#define DEBUG
#include <Servo.h>
#include <Taco.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;
Servo servo;

void setup() {
  RC = new RobotController();
  RC->initialize();
  delay(5000);
  Serial.begin(9600);
  servo.attach(EXTINGUISHER);
  servo.write(180);
  delay(10000);
  servo.write(0);
  delay(2000);
  servo.detach();
}

void loop() {
  // put your main code here, to run repeatedly: No code

} 
