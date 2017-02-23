#include <TaconH.h>

void RobotController::initialize()
{
  motorController = Adafruit_MotorShield(MOTOR_CONTROLLER_ADDR);
  motorController.begin();

  Wire.begin();  // Set up Arduino as I2C master

  for (int i = 0; i < NUM_MOTOR; i++) {
    motor[i].motor = motorController.getMotor(i + MOTOR_ORIGIN);
    motor[i].speed = 0;
    motor[i].direction = RELEASE;
  }
}

void RobotController::go(Direction  heading, int speed, Side sideDirection, int sideSpeed, Rotation turnDirection, int turnSpeed) 
{
  // TODO Implement
}

void RobotController::stop()
{
  for (int i = 0; i < NUM_MOTOR; i++) {
    motor[i].motor = motorController.getMotor(i + MOTOR_ORIGIN);
    motor[i].speed = 0;
    motor[i].direction = BRAKE;
  }
  delay(200); // Braking delay to give time to stop
}

int RobotController::readUv(int sensorId)
{
  int val = analogRead(sensorId);
  int sensorIndex = sensorId - UV_ORIGIN;
  uv[sensorIndex].prevVal = uv[sensorIndex].curVal;
  uv[sensorIndex].curVal = val;
  return val;
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

void RobotController::followWall(Heading wallSide, Heading heading, int speed)
{
  // TODO Implement
}

