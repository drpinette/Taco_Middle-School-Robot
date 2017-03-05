#include <TaconH.h>

void RobotController::initialize()
{
  motorController = Adafruit_MotorShield(MOTOR_CONTROLLER_ADDR);
  motorController.begin();

  Wire.begin();  // Set up Arduino as I2C master

  for (int i = 0; i < NUM_MOTOR; i++) motor[i].run(RELEASE, 0);
}

void RobotController::go(Direction  heading, int speed, Side sideDirection, int sideSpeed, Rotation turnDirection, int turnSpeed) 
{
  // The robot is always going "forward" (in the heading direction).  Compute speeds in a generic 
  // forward direction, then assign speeds to the actual motor controllers for the given heading.
  int speedLeftFront =  -speed + (-(int)sideDirection)*sideSpeed + (-(int)turnDirection)*turnSpeed;
  int speedRightFront =  speed + (-(int)sideDirection)*sideSpeed + (-(int)turnDirection)*turnSpeed;
  int speedLeftBack =   -speed +  ((int)sideDirection)*sideSpeed + (-(int)turnDirection)*turnSpeed;
  int speedRightBack =   speed +  ((int)sideDirection)*sideSpeed + (-(int)turnDirection)*turnSpeed;
  int directionLeftFront = speedLeftFront < 0 ? BACKWARD : FORWARD;
  int directionRightFront = speedRightFront < 0 ? BACKWARD : FORWARD;
  int directionLeftBack = speedLeftBack < 0 ? BACKWARD : FORWARD;
  int directionRightBack = speedRightBack < 0 ? BACKWARD : FORWARD;
  int speedLeftFront = ABS(speedLeftFront);
  int speedRightFront = ABS(speedRightFront);
  int speedLeftBack = ABS(speedLeftBack);  
  int speedRightBack = ABS(speedRightBack);

  // Now assign speeds to motor controllers
  int motorControllerOffset = ((int)heading) - ((int)Heading.North);
  Motor motorLeftFront = motor[motorControllerOffset];
  Motor motorRightFront = motor[(motorControllerOffset+1) % 4];
  Motor motorLeftBack = motor[(motorControllerOffset+2) % 4];
  Motor motorRightBack = motor[(motorControllerOffset+3) % 4];
  motorLeftFront.run(directionLeftFront, speedLeftFront);
  motorRightFront.run(directionRightFront, speedRithFront);
  motorLeftBack.run(directionLeftBack, speedLeftBack);
  motorRightBack.run(directionRightBack, speedRightBack);
}

void RobotController::stop()
{
  for (int i = 0; i < NUM_MOTOR; i++) motor[i].run(BRAKE, 0);
  delay(200); // Braking delay to give time to stop
}

int RobotController::readUv(int sensorId)
{
  int sensorIndex = sensorId - UV_ORIGIN;
  return uv[sensorIndex].update(analogRead(sensorId));
}

float RobotController::readDistanceSonar(int sensorId)
{
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(sensor, OUTPUT);
  digitalWrite(sensor, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensor, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(sensor, INPUT);
  int duration = pulseIn(sensor, HIGH);

  // Convert to inches
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  float distance = ((float)duration) / 74.0 / 2.0;

  int sensorIndex = sensorId - SONAR_ORIGIN;
  sonar[sensorIndex].prevVal = sonar[sensorIndex].curVal;
  sonar[sensorIndex].curVal = val;
  return distance;
}

void RobotController::followWall(Side wallSide, Heading heading, int speed)
{
  // TODO Implement
}

