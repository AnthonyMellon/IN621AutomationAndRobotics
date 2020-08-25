#include <Wire.h>
#include <ZumoShield.h>

ZumoMotors motors;

int maxSpeed = 400;

const int Turn90 = 150;

void setup() {
  
  //spiral();
  //zigzag();
  //lines();
  //three();P
  star();
}

void loop() {
  
  
}

void spiral()
{
  for(int i = 10; i > 0; i--)
  {
    Drive(i*100);
    TurnRight(Turn90);
  }
}

void star()
{
  for(int i = 0; i < 5; i++)
  {
    Drive(500);
    TurnRight(Turn90*1.6);
  }

  
}

void zigzag()
{
  for(int i = 0; i < 6; i++)
  {
    Drive(500);
    if(i%2 == 0)
    {
      TurnRight(Turn90);      
    }
    else
    {
      TurnLeft(Turn90); 
    }
  }
}

void lines()
{
  for(int i =0; i < 2; i++)
  {
    Drive(1000);
    TurnRight(Turn90);
    Drive(100);
    TurnRight(Turn90);

    Drive(1000);
    TurnLeft(Turn90);
    Drive(100);
    TurnLeft(Turn90);
  }
}

void three()
{
    for(int i =0; i < 2; i++)
  {
    Drive(500);
    TurnRight(Turn90);
    Drive(500);
    TurnRight(Turn90);

    Drive(500);
    TurnLeft(Turn90);
    Drive(500);
    TurnLeft(Turn90);
  }
}


void Drive(int duration) {
  motors.setSpeeds(maxSpeed, maxSpeed);
  delay(duration);
  Stop();
}

void Reverse(int duration) {
  motors.setSpeeds(maxSpeed*-1, maxSpeed*-1);
  delay(duration);
  Stop();
}

void TurnLeft(int duration) {
  motors.setSpeeds(maxSpeed*-1, maxSpeed);
  delay(duration);
  Stop();
}

void TurnRight(int duration) {
  motors.setSpeeds(maxSpeed, maxSpeed*-1);
  delay(duration);
  Stop();
}

void Stop()
{
  motors.setSpeeds(0, 0);
}

void testMotors()
{
  Drive(500);
  delay(500);
  Reverse(500);
  delay(500);
  TurnLeft(500);
  delay(500);
  TurnRight(500);
  delay(500);
}
