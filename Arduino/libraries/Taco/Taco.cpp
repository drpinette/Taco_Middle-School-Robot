//#define DEBUG
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
  
  // Scale the speeds so the fastest wheel is always at the default speed
  int maxSpeed = MAX(MAX(speedLeftFront, speedRightFront),MAX(speedLeftBack, speedRightBack));
  float speedScaling = ((float)DEFAULT_SPEED )/ ((float)maxSpeed);
  speedLeftFront = (int)(speedScaling * speedLeftFront);
  speedRightFront = (int)(speedScaling * speedRightFront);
  speedLeftBack = (int)(speedScaling * speedLeftBack);
  speedRightBack = (int)(speedScaling * speedRightBack);

  // Now assign speeds to motor controllers
  int motorControllerOffset = (int)heading - (int)North;
  //Serial.println(motorControllerOffset);
  Motor* motorLeftFront = &(motorArray[motorControllerOffset+0]);
  Motor* motorRightFront = &(motorArray[MOD(motorControllerOffset+1, 4)]);
  Motor* motorRightBack = &(motorArray[MOD(motorControllerOffset+2, 4)]);
  Motor* motorLeftBack = &(motorArray[MOD(motorControllerOffset+3, 4)]);
  motorLeftFront->run(directionLeftFront, speedLeftFront);
  motorRightFront->run(directionRightFront, speedRightFront);
  motorRightBack->run(directionRightBack, speedRightBack);
  motorLeftBack->run(directionLeftBack, speedLeftBack);
  //_DS(motorArray[0].curSpeed);_DS(motorArray[1].curSpeed);_DS(motorArray[2].curSpeed);_DS(motorArray[3].curSpeed);_NL;
  //_DS(motorArray[0].curDirection);_DS(motorArray[1].curDirection);_DS(motorArray[2].curDirection);_DS(motorArray[3].curDirection);_NL;
}

Heading RobotController::rotate(int speed, Rotation turnDirection, Condition* stopCondition)
{
  int initialUv[NUM_UV];
  int maxUv[NUM_UV];
  for (int i = (int)North; i < NUM_UV; i++) initialUv[i] = maxUv[i] = readUv(uvIdAt((Heading)i));
  go(North, 0, NoSide, 0, turnDirection, speed);
  while(stopCondition != NULL && !stopCondition->test())
  {
    for (int i = (int)North; i < NUM_UV; i++) maxUv[i] = MAX(maxUv[i], readUv(uvIdAt((Heading)i)));
  }
  Heading maxUvHeading = North;
  int maxUvChange = maxUv[(int)North] - initialUv[(int)North];
  for (int i = ((int)North)+1; i < NUM_UV; i++) 
  {
    int curUvChange = maxUv[i] - initialUv[i];
    if (curUvChange > maxUvChange) 
    {
      maxUvChange = curUvChange;
      maxUvHeading = (Heading)i;
    }
  }
  return maxUvHeading;
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
  // Wait for any other sonar activity to die down
  delay(SONAR_REFRACTORY_PERIOD);  //added delay to prevent signal interference

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
  sonarArray[sensorIndex].prevVal = sonarArray[sensorIndex].curVal;
  sonarArray[sensorIndex].curVal = distance;
  return distance;
}

void RobotController::followWall(Side wallSide, Heading heading, int speed, Condition* stopCondition, Rotation ignoreSonarAt)
{
  int sonarOffset = (int)heading - (int)North;
  int wallOffset = wallSide == Left ? 4: 0;
  int sonarPinCCW = MOD(2+wallOffset + 2*sonarOffset, 8) + SONAR_ORIGIN;
  int sonarPinCW = MOD(3+wallOffset + 2*sonarOffset, 8) + SONAR_ORIGIN;
 
  // Correct robot's location and orientation until this condition is met
  while(stopCondition != NULL && !stopCondition->test())
  {
    // Get distances to wall for each sonar
    float distanceCCW = readDistanceSonar(sonarPinCCW);
    float distanceCW = readDistanceSonar(sonarPinCW);
    _D(distanceCCW); _NL; _D(distanceCW); _NL;

    // Suppress angle correction, if requested
    if (ignoreSonarAt == CCW) distanceCCW = distanceCW;
    else if (ignoreSonarAt == CW) distanceCW = distanceCCW;

    // Correct robot's location
    float distanceAver = (distanceCCW + distanceCW)/2;
    float sideDifference = WALL_SAFETY_MARGIN - distanceAver;
    float angleDifference = distanceCCW - distanceCW;
    Side sideDirection = sideDifference > 0 ? (wallSide == Right ? Left : Right) : (wallSide == Right ? Right : Left);
    Rotation turnDirection = (Rotation)SGN(angleDifference);
    int sideSpeed = (int)ABS(SIDE_CORRECTION_FACTOR * speed * (sideDifference / WALL_SAFETY_MARGIN));
    int turnSpeed = (int)ABS(TURN_CORRECTION_FACTOR * speed * (angleDifference / ANGLE_SAFETY_MARGIN));
    _D(turnDirection); _D(turnSpeed); _D(sideSpeed); _D(sideDirection); _NL;
    go(heading, speed, sideDirection, sideSpeed, turnDirection, turnSpeed);
  }

  // Delete the condition object, since we're done with it
  if (stopCondition != NULL) delete stopCondition;
}

int RobotController::sonarIdAt(Heading heading, Side side, Rotation direction)
{
  int sonarOffset = MOD((((int)heading - (int)North) + (int)side) * 2 + (direction == CCW ? 0 : 1), NUM_SONAR);
  return SONAR_ORIGIN + sonarOffset;
}

int RobotController::uvIdAt(Heading heading)
{
  int uvOffset = MOD((int)heading - (int)North, NUM_UV);
  return UV_ORIGIN + uvOffset;
}

void RobotController::move(Heading heading, int speed, Condition* stopCondition)
{
	float avoidMargin = 2.0;
	int sonarOffset = (int)heading - (int)North;
	int sonarPinLeft= MOD(-1 + 2*sonarOffset, 8) + SONAR_ORIGIN;
	int sonarPinRight = MOD(2 + 2*sonarOffset, 8) + SONAR_ORIGIN;
	while (!stopCondition->test())
	{
		Side sideDirection = NoSide;
		int sideSpeed = 0;
		float distanceLeft = readDistanceSonar(sonarPinLeft);
		if (distanceLeft < avoidMargin) {
			float sideDifference = avoidMargin - distanceLeft;
			sideDirection = Right;
			sideSpeed = (int)ABS(SIDE_CORRECTION_FACTOR * speed * (sideDifference / avoidMargin));
		}
		else{
			float distanceRight = readDistanceSonar(sonarPinRight);
			if (distanceRight < avoidMargin){
				float sideDifference = avoidMargin - distanceRight;
				sideDirection = Left;
				sideSpeed = (int)ABS(SIDE_CORRECTION_FACTOR * speed * (sideDifference / avoidMargin));
			}
		}
		_D(sideSpeed); _D(sideDirection); _NL;
		go(heading, speed, sideDirection, sideSpeed, NoRotation, 0);
	}
}

void RobotController::extinguish(bool on)
{
	servo.attach(EXTINGUISHER);
	servo.write(on ? 180 : 0);
	delay(on ? 20000 : 2000);
	servo.detach();
}
