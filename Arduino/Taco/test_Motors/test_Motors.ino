#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Middle_School_Robot.h>

#define LEFT_FRONT_MOTOR 2
#define RIGHT_FRONT_MOTOR 1
#define LEFT_BACK_MOTOR 3
#define RIGHT_BACK_MOTOR 4

// initlize motor 
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor* leftFrontMotor = AFMS.getMotor(LEFT_FRONT_MOTOR);
Adafruit_DCMotor* rightFrontMotor = AFMS.getMotor(RIGHT_FRONT_MOTOR);
Adafruit_DCMotor* leftBackMotor = AFMS.getMotor(LEFT_BACK_MOTOR);
Adafruit_DCMotor* rightBackMotor = AFMS.getMotor(RIGHT_BACK_MOTOR);


void setup(){
  AFMS.begin();
  
  delay(5000);
}


void loop(){
  moveForward(150);
  pause(1500);
  moveRight(150);
  pause(1500);
  moveBackward(150);
   pause(1500);
   moveLeft(150);
   pause(1500);
}

void moveForward(int speed){
     rightFrontMotor->run(BACKWARD);
     leftBackMotor->run(FORWARD);
     leftFrontMotor->run(FORWARD);
     rightBackMotor->run(BACKWARD);
     rightFrontMotor->setSpeed(speed);
   leftFrontMotor->setSpeed(speed);
   rightBackMotor->setSpeed(speed);
   leftBackMotor->setSpeed(speed);
}  
   void moveRight(int speed){
     rightFrontMotor->run(FORWARD);
     leftBackMotor->run(BACKWARD);
     leftFrontMotor->run(FORWARD);
     rightBackMotor->run(BACKWARD);
     rightFrontMotor->setSpeed(speed);
   leftFrontMotor->setSpeed(speed);
   rightBackMotor->setSpeed(speed);
   leftBackMotor->setSpeed(speed);
   }
void moveBackward(int speed){
     rightFrontMotor->run(FORWARD);
     leftBackMotor->run(BACKWARD);
     leftFrontMotor->run(BACKWARD);
     rightBackMotor->run(FORWARD);
     rightFrontMotor->setSpeed(speed);
   leftFrontMotor->setSpeed(speed);
   rightBackMotor->setSpeed(speed);
   leftBackMotor->setSpeed(speed);
} 
   void moveLeft(int speed){
     rightFrontMotor->run(BACKWARD);
     leftBackMotor->run(FORWARD);
     leftFrontMotor->run(BACKWARD);
     rightBackMotor->run(FORWARD);
     rightFrontMotor->setSpeed(speed);
   leftFrontMotor->setSpeed(speed);
   rightBackMotor->setSpeed(speed);
   leftBackMotor->setSpeed(speed);
   }

void pause(int time){
  delay(time);
   rightFrontMotor->setSpeed(0);
   leftFrontMotor->setSpeed(0);
   rightBackMotor->setSpeed(0);
   leftBackMotor->setSpeed(0);
}
