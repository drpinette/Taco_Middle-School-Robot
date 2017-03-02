#include <Motor.h>

public void Motor::run(int direction, int speed)
{
  motor->run(direction);
  motor->setSpeed(speed);
}
