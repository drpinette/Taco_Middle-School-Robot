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
 while(digitalRead(START) == LOW)
 {
  
 }
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
    if(dog == true)
    {
      RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 4.0, RC));
    }
    
    if(dog == false)
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
      delay(99999999999999999999999999999999999999999999);
    }
    RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 5.0, RC));
    RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 5.0, RC));
  }

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
  RC->move(North, 50, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 6.0, RC));
  
   //Checking 1st Room//////////////////////////////////////////////////////////////////////////////////////////////////////////////
   RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, 17.0, RC));
 
   if (RC->readUv(EAST_UV) > maxUv || RC->readUv(SOUTH_UV) > maxUv)
   {
    RC->stop();
    digitalWrite(13, HIGH);
    if (RC->readUv(EAST_UV) > RC->readUv(SOUTH_UV))
    {
      RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, 5.0, RC));
      RC->stop();
      RC->extinguish(true);
    }
    else
    {
      RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CCW), LessThan, 12.0, RC));
      RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CW), LessThan, 12.0, RC));
      RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 5.0, RC));
      RC->stop();
      if (RC->readUv(EAST_UV) < RC->readUv(SOUTH_UV))
      {
        float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(South, NoSide, CW));
        RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, distanceToWall, RC));
        RC->stop();
        RC->extinguish(true);
      }
      else
      {
        RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 5.0, RC));
        RC->stop();
        float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(South, Left, CW));
        RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, (distanceToWall + 1), RC));
        RC->stop();
        RC->extinguish(true);
      }
    }
   }
   RC->stop();
   RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), MoreThan, 35.0, RC));
   RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 35.0, RC));
   RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 35.0, RC));
   RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 37.4, RC));

  //checking second room////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (RC->readUv(SOUTH_UV) > maxUv)
   {
    digitalWrite(13, HIGH);
    RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), LessThan, 12.0, RC));
    RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), LessThan, 12.0, RC));
    RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 5.0, RC));
    RC->stop();
    float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(South, Left, CW));
    RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, (distanceToWall + 1), RC));
    RC->stop();
    RC->extinguish(true);
   }
   
  else
  {
    RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 5.0, RC));
    RC->stop();
    if (RC->readUv(WEST_UV) > maxUv)
    {
     digitalWrite(13,HIGH);
     float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, NoSide, CCW));
     RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, (distanceToWall + 1), RC));
     RC->stop();
     RC->extinguish(true);
    }
   }
   RC->go(East, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
   delay(500);
   RC->stop();
   RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 34.0, RC));
   RC->stop();
  
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 12.0, RC));
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), MoreThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), MoreThan, 12.0, RC));
  RC->go(South, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(100);
  RC->stop();
  //in intersection
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 12.0, RC));
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, 12.0, RC));
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 6.0, RC));
//moving into third room////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 24.0, RC));
 
   if (RC->readUv(WEST_UV) > maxUv)
   {
    RC->stop();
    digitalWrite(13, HIGH);
    RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 5.0, RC));
    RC->stop();
    float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, Right, CCW));
    RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, distanceToWall, RC));
    RC->stop();
    RC->extinguish(true);
   }
  else
  {
    RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 5.0, RC));
    if (RC->readUv(SOUTH_UV) > maxUv || RC->readUv(WEST_UV) > maxUv)
    {
     RC->stop();
     digitalWrite(13,HIGH);
     if (RC->readUv(SOUTH_UV) > RC->readUv(WEST_UV))
     {
      float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, Right, CW));
      RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, (distanceToWall + 1), RC));
      RC->stop();
      RC->extinguish(true);
     }
     else
     {
      RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 5.0, RC));
      RC->stop();
      float distanceToWall = RC->readDistanceSonar(RC->sonarIdAt(North, Left, CCW));
      RC->rotate(DEFAULT_SPEED, CCW, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, (distanceToWall + 1), RC));
      RC->stop();
      RC->extinguish(true);
     }
    }
   }
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
      delay(99999999999999999999999999999999999999999999);
    }
  RC->stop();
 
  
}

void loop() {
  // don't use
} 
