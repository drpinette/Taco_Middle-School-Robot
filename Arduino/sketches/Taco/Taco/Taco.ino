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
  Serial.begin(9600);
  bool dog = false;
  RC->extinguish(false);

   //wait for button
   while(digitalRead(START) == LOW);

  delay(100);
  int maxUv = 10 + MAX(MAX( RC->readUv(NORTH_UV), RC->readUv(SOUTH_UV)), MAX(RC->readUv(EAST_UV), RC->readUv(WEST_UV)));
  if(RC->readDistanceSonar(EAST_CCW_SONAR) < 12 && RC->readDistanceSonar(EAST_CW_SONAR) < 12)
  {
    //In Correct Starting Position - Do nothing
    Serial.println("Detected facing North");
  }
  else
  {
    Serial.println("Detected facing East");
    RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 4.0, RC));
    RC->stop();
  }

  //Check for dog
  //dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  //dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 4.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 5.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 6.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 7.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 3.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CW)) < 10;
  RC->stop();
  if(dog == false)
  {
  RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), MoreThan, 5.5, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 4.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 5.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 6.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 3.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  RC->stop();
  }

  // start navigating

    // Return to starting location if there is a dog
    if(dog == true)
    {
      RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 4.0, RC));
    }
    // Check out room 4 if there is not dog
    else
    {
    Serial.println("No dog");
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), LessThan, 8.5, RC));
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), LessThan, 8.5, RC));
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 8.5, RC));
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), MoreThan, 8.5, RC));
    //moving into room four/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
    if (RC->readUv(NORTH_UV) > maxUv || RC->readUv(EAST_UV) > maxUv)
    {
      RC->stop();
      digitalWrite(13, HIGH);
      if (RC->readUv(NORTH_UV) > RC->readUv(EAST_UV)) {
        float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CW));
        RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, (distanceToWall+1.5), RC));
        RC->stop();
        RC->extinguish(true);
      }
      else{
        RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 5.0, RC));
        RC->stop();
        if (RC->readUv(NORTH_UV) > RC->readUv(SOUTH_UV)) 
        {
          RC->extinguish(true);
        }
        else
        {
          float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CCW));
          RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, (distanceToWall + 1), RC));
          RC->stop();
          RC->extinguish(true);
        }
      }
      return;
    }
    RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 5.0, RC));
    RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 5.0, RC));
  }

    // Moving north from home
    RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 8.5, RC));
    RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, WALL_SAFETY_MARGIN + 1.0, RC), CCW);
    RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
    RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, WALL_SAFETY_MARGIN + 1.0, RC), CW);
    RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 12.0, RC));
    RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, WALL_SAFETY_MARGIN + 1.0, RC), CCW);
    RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 6.0, RC));

    // Moving into first room
    RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CCW), LessThan, 48.0, RC));
    RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CW), LessThan, 48.0, RC));
    RC->followWall(Left, East, DEFAULT_SPEED, new MovedBy(6.0, East, RC));

    //Checking 1st Room//////////////////////////////////////////////////////////////////////////////////////////////////////////////   
    if (RC->readUv(EAST_UV) > maxUv || RC->readUv(SOUTH_UV) > maxUv)
    {
      // Scan for corner with brightest UV
      Heading uvHeading = RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CCW)), RC));
      RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)), RC));

      // Move to corner with brightest UV and point at corner
      switch (uvHeading) {
       case North:
         RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 5.0, RC));
         break;
       case East:
         RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 5.0, RC));
         RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 5.0, RC));
         RC->stop();
         RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(East, NoSide, CCW)), RC));
         break;
       case South:
         RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), LessThan, 12.0, RC));
         RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CW), LessThan, WALL_SAFETY_MARGIN + 1.0, RC), CW);
         RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 5.0, RC));
         RC->stop();
         RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(South, NoSide, CCW)), RC));
         RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(East, NoSide, CCW)), RC));
       break;
      }
      // Extinguish candle
      RC->stop();
      RC->extinguish(true);
      return;
    }

    // Move out of first room into second room
    RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Left, CW), MoreThan, 48.0, RC));
    RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Left, CCW), MoreThan, 48.0, RC));
    RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Left, CW), LessThan, 48.0, RC));
    RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Left, CCW), LessThan, 48.4, RC));
    RC->followWall(Right, West, DEFAULT_SPEED, new MovedBy(6.0, West, RC));

    //checking second room////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (RC->readUv(SOUTH_UV) > maxUv || RC->readUv(WEST_UV))
    {
      // Scan for corner with brightest UV
      Heading uvHeading = RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CCW)), RC));
      RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)), RC));

      // Move to corner with brightest UV and point at corner
      switch (uvHeading) {
       case West:
         RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 5.0, RC));
         RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CCW)), RC));
         RC->stop();
         break;
       case East:
         RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), LessThan, 12.0, RC));
         RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), LessThan, WALL_SAFETY_MARGIN + 1.0, RC), CCW);
         RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 5.0, RC));
         RC->stop();
         RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(East, NoSide, CCW)), RC));
       break;
      }
      // Extinguish candle
      RC->stop();
      RC->extinguish(true);
      return;
    }

    // Leave second room
    RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CW), MoreThan, 48.0, RC));
    RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CCW), MoreThan, 48.0, RC));
    RC->followWall(Left, East, DEFAULT_SPEED, new MovedBy(6.0, West, RC));
    RC->stop();

    // Go down the hall
    RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), LessThan, 12.0, RC));
    RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), LessThan, WALL_SAFETY_MARGIN + 1.0, RC), CCW);
    RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), MoreThan, 8.0, RC));
    RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), MoreThan, WALL_SAFETY_MARGIN + 1.0, RC), CCW);
    RC->move(South, DEFAULT_SPEED, new MovedBy(6.0, South, RC));
    RC->stop();
    
    // Now we're in the intersection.  Go to third room

    //in intersection, moving down hall to third room
    RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 12.0, RC));
    RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, WALL_SAFETY_MARGIN + 1.0, RC), CCW);
    RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, 5.0, RC));

   //moving into third room////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CCW), LessThan, 48.0, RC));
    RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CW), LessThan, 48.0, RC));
    RC->followWall(Left, South, DEFAULT_SPEED, new MovedBy(6.0, East, RC));
 
    if (RC->readUv(WEST_UV) > maxUv || RC->readUv(SOUTH_UV) > maxUv)
    {
      // Scan for corner with brightest UV
      Heading uvHeading = RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(East, NoSide, CCW)), RC));
      RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(East, NoSide, CW)), RC));

      // Move to corner with brightest UV and point at corner
      switch (uvHeading) {
       case East:
         RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 5.0, RC));
         RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(East, NoSide, CCW)), RC));
         RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CCW)), RC));
         break;
       case South:
         RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 5.0, RC));
         RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 5.0, RC));
         RC->stop();
         RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 0.25 + RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CW)), RC));
         break;
       case West:
         RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), LessThan, 12.0, RC));
         RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), LessThan, WALL_SAFETY_MARGIN + 1.0, RC), CW);
         RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 5.0, RC));
         RC->stop();
       break;
      }
      // Extinguish candle
      RC->stop();
      RC->extinguish(true);
      return;
    }

    // TODO: Premature return, implement the rest of this
    return;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), MoreThan, 12.0, RC));
  RC->go(North, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(100);
  RC->stop();
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), LessThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), LessThan, 12.0, RC));
  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 6.0, RC));
  RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 6.0, RC));
  RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 35.0, RC));
  RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, 35.0, RC));
  RC->go(East, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(400);
  RC->stop();
  //moving into room four///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
    if (RC->readUv(NORTH_UV) > maxUv || RC->readUv(EAST_UV) > maxUv)
    {
      RC->stop();
      digitalWrite(13, HIGH);
      if (RC->readUv(NORTH_UV) > RC->readUv(EAST_UV)) {
        float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CW));
        RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, (distanceToWall+1.5), RC));
        RC->stop();
        RC->extinguish(true);
      }
      else{
        RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 5.0, RC));
        RC->stop();
        if (RC->readUv(NORTH_UV) > RC->readUv(SOUTH_UV)) 
        {
          RC->extinguish(true);
        }
        else
        {
          float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CCW));
          RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, (distanceToWall + 1), RC));
          RC->stop();
          RC->extinguish(true);
        }
      }
      return;
    }
  RC->stop();
 
  
}

void loop() {
  // don't use
} 
