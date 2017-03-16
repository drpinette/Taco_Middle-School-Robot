#include <Taco.h>

void RobotController::initialize()
{
  motorController = Adafruit_MotorShield(MOTOR_CONTROLLER_ADDR);
  for (int i = 0; i < NUM_MOTOR; i++) motorArray[i].motor = motorController.getMotor(i+1);
  motorController.begin();

  Wire.begin();  // Set up Arduino as I2C master

  for (int i = 0; i < NUM_MOTOR; i++) motorArray[i].run(RELEASE, 0);
}

void RobotController::go(Heading  heading, int speed, Side sideDirection, int sideSpeed, Rotation turnDirection, int turnSpeed) 
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
  _D(speedLeftFront);_D(speedRightFront);_D(speedRightBack);_D(speedLeftBack);_NL;
  _D(directionLeftFront);_D(directionRightFront);_D(directionLeftBack);_D(directionRightBack);_NL;
 speedLeftFront = ABS(speedLeftFront);
 speedRightFront = ABS(speedRightFront);
 speedLeftBack = ABS(speedLeftBack);  
 speedRightBack = ABS(speedRightBack);

  // Now assign speeds to motor controllers
  int motorControllerOffset = ((int)heading) - ((int)(Heading::North));
  Serial.println(motorControllerOffset);
  Motor motorLeftFront = motorArray[motorControllerOffset+0];
  Motor motorRightFront = motorArray[(motorControllerOffset+1) % 4];
  Motor motorRightBack = motorArray[(motorControllerOffset+2) % 4];
  Motor motorLeftBack = motorArray[(motorControllerOffset+3) % 4];
  motorLeftFront.run(directionLeftFront, speedLeftFront);
  motorRightFront.run(directionRightFront, speedRightFront);
  motorRightBack.run(directionRightBack, speedRightBack);
  motorLeftBack.run(directionLeftBack, speedLeftBack);
  _DS(motorArray[0].curSpeed);_DS(motorArray[1].curSpeed);_DS(motorArray[2].curSpeed);_DS(motorArray[3].curSpeed);_NL;
  _DS(motorArray[0].curDirection);_DS(motorArray[1].curDirection);_DS(motorArray[2].curDirection);_DS(motorArray[3].curDirection);_NL;
}

void RobotController::stop()
{
  for (int i = 0; i < NUM_MOTOR; i++) motorArray[i].run(BRAKE, 0);
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
  pinMode(sensorId, OUTPUT);
  digitalWrite(sensorId, LOW);
  delayMicroseconds(2);
  digitalWrite(sensorId, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensorId, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(sensorId, INPUT);
  int duration = pulseIn(sensorId, HIGH);

  // Convert to inches
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  float distance = ((float)duration) / 74.0 / 2.0;

  int sensorIndex = sensorId - SONAR_ORIGIN;
  sonar[sensorIndex].prevVal = sonar[sensorIndex].curVal;
  sonar[sensorIndex].curVal = distance;
  return distance;
}

void RobotController::followWall(Side wallSide, Heading heading, int speed)
{
  // TODO Implement
}

