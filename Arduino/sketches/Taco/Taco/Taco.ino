//#define DEBUG
#include <Taco.h>
#include <Condition.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>


RobotController* RC;

void setup() {
  pinMode(13, OUTPUT);
  RC = new RobotController();
  RC->initialize();
  delay(5000);
  Serial.begin(9600);

  if(RC->readDistanceSonar(EAST_CCW_SONAR) < 12 && RC->readDistanceSonar(EAST_CW_SONAR) < 12)
  {
    //In Correct Starting Position - Do nothing
    Serial.println("Detected facing North");
  }
  else
  {
    Serial.println("Detected facing East");
    RC->rotate(DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 4.0, RC));
    RC->stop();
  }


  // start navigating
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 8.5, RC));
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), MoreThan, 12.0, RC));
  //RC->stop();
  //delay(5000);
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
  //RC->stop();
  //delay(5000);
  //_DS("========================checkPoint1"); _NL; 
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 12.0, RC));
  //_DS("========================checkPoint2"); _NL;
  //RC->stop();
  //delay(5000);
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 12.0, RC));
  //RC->stop();
  //delay(5000);
  //_DS("========================checkPoint3"); _NL;
  RC->move(North, 50, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 4.0, RC));
  
  //Checking 1st Room
   RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, 21.0, RC));
   RC->stop();
   if (RC->readUv(EAST_UV) > 80 || RC->readUv(SOUTH_UV) > 80)
   {
    digitalWrite(13, HIGH);
    delay(999999999999999999999999999999999999999999999999999999999999999999);
   }
   else
   {
   RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 32.0, RC));
   RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 32.0, RC));
   RC->stop();
   }

  //checking second room
    if (RC->readUv(WEST_UV) > 80 || RC->readUv(SOUTH_UV) > 80)
   {
    digitalWrite(13, HIGH);
    delay(999999999999999999999999999999999999999999999999999999999999999999);
   }
   else
   {
   RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 34.0, RC));
   RC->stop();
   }
  
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 12.0, RC));
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), MoreThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), MoreThan, 12.0, RC));
  RC->move(South, 150, new DistanceFor(RC->sonarIdAt(South, Right, CCW), LessThan, 12.0, RC));
  RC->move(North, 100, new DistanceFor(RC->sonarIdAt(South, Right, CCW), MoreThan, 12.0, RC));
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 12.0, RC));
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, 12.0, RC));
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 4.0, RC));
  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), MoreThan, 12.0, RC));
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), LessThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), LessThan, 12.0, RC));
  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 4.0, RC));
  RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 4.0, RC));
  RC->stop();
 
  
}

void loop() {
  // don't use

} 
