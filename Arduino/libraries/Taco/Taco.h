#ifndef TACO_H
#define TACO_H
/////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Sensor.h>
#include <Motor.h>
#include <Condition.h>

#define DEBUG
//#undef DEBUG

enum Rotation { NoRotation = 0, CW  = 1 /*Clockwise*/, CCW = -1 /*Counterclockwise*/ };
enum Heading { North = 0, East = 1, South = 2, West = 3 };
enum Side { NoSide = 0, Right = 1, Left = -1 };

#ifdef DEBUG
#define _D(X) Serial.print(#X " "), Serial.print(X), Serial.print("; ") 
#define _DS(X) Serial.print( " "), Serial.print(X), Serial.print("; ")
#define _DC(X) Serial.print(#X "; ")
#define _NL Serial.print("\n")
#else
#define _D
#define _DC
#define _NL
#endif

// Digital I/O pin assignments
#define AVAILABLE0 0
#define AVAILABLE1 1
#define AVAILABLE2 12
#define START 11
#define NORTH_CCW_SONAR 2
#define NORTH_CW_SONAR 3
#define EAST_CCW_SONAR 4
#define EAST_CW_SONAR 5
#define SOUTH_CCW_SONAR 6
#define SOUTH_CW_SONAR 7
#define WEST_CCW_SONAR 8
#define WEST_CW_SONAR 9
#define EXTINGUISHER 10
#define LED 13

#define NUM_SONAR 8
#define SONAR_ORIGIN 2

// Analog I/O pin assignments
#define NORTH_UV 0
#define EAST_UV 1
#define SOUTH_UV 2
#define WEST_UV 3
#define UNASSIGNED1 4
#define UNASSIGNED2 5

#define NUM_UV 4
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

#define MAX_SPEED 255
#define TURN_CORRECTION_FACTOR 0.2
#define SIDE_CORRECTION_FACTOR 0.2
#define DEFAULT_SPEED ((int)(MAX_SPEED*(1.0-TURN_CORRECTION_FACTOR-SIDE_CORRECTION_FACTOR)))  //current 153
#define WALL_SAFETY_MARGIN 4.0

#define ABS(x) ((x)<0 ? -(x) : (x))
#define SGN(x) ((x)<0 ? -1 : ((x)>0 ? 1 : 0))
#define MOD(x, m) (((x)%(m))<0 ? ((x)%(m))+m : ((x)%(m)))

class RobotController
{
public:
  void initialize();
  void go(Heading  heading, int speed, Side sideDirection, int sideSpeed, Rotation turnDirection, int turnSpeed); 
  void stop();
  int readUv(int sensorId);
  float readDistanceSonar(int sensorId);
  void followWall(Side wallSide, Heading heading, int speed, Condition* stopCondition);
  void move(Heading heading, int speed, Condition* stopCondition);
  int sonarIdAt(Heading heading, Side side, Rotation direction);
  int uvIdAt(Heading heading);


private:
  Sensor sonarArray[NUM_SONAR];
  Sensor uv[NUM_UV];
  Motor motorArray[NUM_MOTOR];
  Adafruit_MotorShield motorController;
};


/////////////////////////////////////////////////////////////////////////////
#endif
