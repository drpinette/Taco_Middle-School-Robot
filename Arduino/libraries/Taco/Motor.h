#ifndef MOTOR
#define MOTOR
//////////////////////////////////////////////////////////////////////////////
#include <Adafruit_MotorShield.h>

class Motor
{
public:
  Adafruit_DCMotor* motor;
  int speed;
  int direction;
  void run(int direction, int speed);
  Motor();
};

//////////////////////////////////////////////////////////////////////////////
#endif
