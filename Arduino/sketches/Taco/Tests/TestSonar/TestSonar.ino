#include <Taco.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
}

void loop() {
  RC->readDistanceSonar(sensorId);
  Serial.println(RC->sonarBL.curVal);
  delay(1000);
}
