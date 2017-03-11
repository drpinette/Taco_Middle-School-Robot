#include <Taco.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  Serial.begin(9600);
}

int maxDelay = 0;

void loop() {
    int val = RC->readUv(WEST_UV);
    Serial.println(val);
    if (val > maxDelay) maxDelay = val;
    if (val > 0) {
      digitalWrite(LED, HIGH);
      delay(val/10);
      digitalWrite(LED, LOW);
    }
    delay((maxDelay-val)/10);
}

