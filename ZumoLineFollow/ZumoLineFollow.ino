#include <Wire.h>
#include <ZumoShield.h>

#define NUM_SENSORS 6
#define MAX_REFLECTANCE 2000

ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;

int maxSpeed = 100;
int leftValue;
int rightValue;

const int Turn90 = 150;

unsigned int sensor_values[NUM_SENSORS];

enum state {
  followingLine,
  lookingForLine,
  backTracking
};
state roboState;

void followLine()
{
  leftValue = 0;
  rightValue = 0;
  for(int i = 0; i < NUM_SENSORS/2; i++) //how much line is on the left?
  {
    leftValue += sensor_values[i];
  }

  for(int i = NUM_SENSORS/2; i < NUM_SENSORS; i++) //how much line is on the right?
  {
    rightValue += sensor_values[i];
  }  

  motors.setSpeeds(scale(rightValue, MAX_REFLECTANCE*(NUM_SENSORS/3), maxSpeed), scale(leftValue, MAX_REFLECTANCE*(NUM_SENSORS/3), maxSpeed));
}

void findLine()
{

}

void backTrack()
{

}

void spin()
{
  motors.setSpeeds(400, -400);
}

int scale (int value, int maxOld, int maxNew)
{
  return (value / (maxOld / maxNew));
}

void debug()
{
  Serial.print(leftValue);
  Serial.print(" | ");
  Serial.print(rightValue);
  Serial.println();
  Serial.print(leftValue / (MAX_REFLECTANCE*3/maxSpeed));
  Serial.print(" | ");
  Serial.print(rightValue / (MAX_REFLECTANCE*3/maxSpeed));
  Serial.println();
  Serial.println("---------------------------------");
}

//Setup Method
void setup() {
  Serial.begin(9600);
  reflectanceSensors.init();
}

//Main Loop
void loop() {    
  reflectanceSensors.read(sensor_values);

  switch (roboState)
  {
    case followingLine:
      followLine();
      break;

    case lookingForLine:
      findLine();
      break;

    case backTracking:
      backTrack();
      break;

    default:
      spin();
      break;
  }
  debug();
}