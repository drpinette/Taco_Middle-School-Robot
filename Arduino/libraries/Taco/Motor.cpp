#include <Motor.h>

Motor::Motor() : direction(RELEASE), speed(0), motor(NULL)
{
}

void Motor::run(int direction, int speed)
{
  this->direction = direction;
  this->speed = speed;
  motor->run(direction);
  motor->setSpeed(speed);
}
