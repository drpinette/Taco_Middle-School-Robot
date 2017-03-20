#include <Motor.h>

Motor::Motor() : curDirection(RELEASE), curSpeed(0), motor(NULL)
{
}

void Motor::run(int direction, int speed)
{
  curDirection = direction;
  curSpeed = speed;
  Serial.print(curSpeed);
  Serial.print(curDirection);
  motor->run(direction);
  motor->setSpeed(speed);
  
}
