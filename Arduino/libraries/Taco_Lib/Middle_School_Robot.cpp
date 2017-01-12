#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Middle_School_Robot.h>

// initlize motor 
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor* leftFrontMotor = AFMS.getMotor(LEFT_FRONT_MOTOR);
Adafruit_DCMotor* rightFrontMotor = AFMS.getMotor(RIGHT_FRONT_MOTOR);
Adafruit_DCMotor* leftBackMotor = AFMS.getMotor(LEFT_BACK_MOTOR);
Adafruit_DCMotor* rightBackMotor = AFMS.getMotor(RIGHT_BACK_MOTOR);


int moveForward(int speed){
	RIGHT_FRONT_MOTOR
}