#ifndef PREPARATION_H
#define PREPARATION_H
/////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Sensor.h>
#include <Motor.h>

//#define DEBUG
//#undef DEBUG

enum Rotation { None = 0, CW  = 1 /*Clockwise*/, CCW = -1 /*Counterclockwise*/ };
enum Heading { None = 0, North = 1, East = 2, South = 3, West = 4 };
enum Side { None = 0, Right = 1, Left = -1 };

#ifdef DEBUG
#define _D(X) Serial.print(#X " "), Serial.print(X), Serial.print("; ") 
#define _DC(X) Serial.print(#X "; ")
#define _NL Serial.print("\n")
#else
#define _D
#define _DC
#define _NL
#endif

// Digital I/O pin assignments
#define UNAVAILABLE 0
#define UNAVAILABLE 1
#define UNAVAILABLE 2
#define UNAVAILABLE 3
#define NORTH_CCW_SONAR 4
#define NORTH_CW_SONAR 5
#define EAST_CCW_SONAR 6
#define EAST_CW_SONAR 7
#define SOUTH_CCW_SONAR 8
#define SOUTH_CW_SONAR 9
#define WEST_CCW_SONAR 10
#define WEST_CW_SONAR 11
#define EXTINGUISHER 12
#define LED 13

#define NUM_SONAR 8
#define SONAR_ORIGIN 4

// Analog I/O pin assignments
#define UV0 0
#define UV1 1
#define UV2 2
#define UV3 3
#define UV4 4
#define UNASSIGNED 5

#define NUM_UV 5
#define UV_ORIGIN 0

// I2C address assignments
#define MOTOR_CONTROLLER_ADDR 0x60

// Motor controller  assignments
#define NORTH_CCW_MOTOR_ID 1
#define NORTH_CW_MOTOR_ID 2
#define EAST_CCW_MOTOR_ID 2
#define EAST_CW_MOTOR_ID 3
#define SOUTH_CCW_MOTOR_ID 3
#define SOUTH_CW_MOTOR_ID 4
#define WEST_CCW_MOTOR_ID 4
#define WEST_CW_MOTOR_ID 1

#define NUM_MOTOR 4
#define MOTOR_ORIGIN 1

#define MAX_SPEED 1023
#define DEFAULT_SPEED (MAX_SPEED/2)

#define ABS(x) ((x)<0 ? -(x) : (x))

class RobotController
{
public:
  void initialize();
  void go(Heading  heading, int speed, Side sideDirection, int sideSpeed, Rotation turnDirection, int turnSpeed); 
  void stop();
  int readUv(int sensor);
  float readDistanceSonar(int sensor);
  void followWall(Direction wallSide, Direction heading, int speed=DEFAULT_SPEED);

private:
  Sensor sonar[NUM_SONAR];
  Sensor uv[NUM_UV];
  Motor motor[NUM_MOTOR];
  Adafruit_MotorShield motorController;
};


/////////////////////////////////////////////////////////////////////////////
#endif