#pragma region Includes
#include <Wire.h>
#include <ZumoShield.h>
#pragma endregion Includes


#pragma region Variable Declarations
#define NUM_SENSORS 6
#define MAX_REFLECTANCE 2000
#define LINE_THRESHOLD 3500

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
#pragma endregion Variable Declarations


#pragma region State Methods
void followLine()
{
  updateLineValues();
  motors.setSpeeds(scale(rightValue, MAX_REFLECTANCE*(NUM_SENSORS/3), maxSpeed), scale(leftValue, MAX_REFLECTANCE*(NUM_SENSORS/3), maxSpeed));
  checkState();
}

void findLine()
{
  updateLineValues();
  if (leftValue == MAX_REFLECTANCE*3)
  {
    motors.setSpeeds(0, 400);    
  }
  else if (rightValue == MAX_REFLECTANCE*3)
  {
    motors.setSpeeds(400, 0);    
  }
  else if (rightValue < LINE_THRESHOLD && leftValue < LINE_THRESHOLD)
  {
    motors.setSpeeds(-400, -400);
  }
  else if (rightValue == MAX_REFLECTANCE*3 && leftValue == MAX_REFLECTANCE*3)
  {
    motors.setSpeeds(400, 0);
  }
  updateLineValues();
  checkState();
}

void backTrack()
{
  Serial.println("Back Track");
}

void checkState()
{
    if(leftValue > LINE_THRESHOLD && rightValue > LINE_THRESHOLD)
    {
      roboState = followingLine;
    }
    else if(leftValue < LINE_THRESHOLD && rightValue < LINE_THRESHOLD)
    {
      roboState = lookingForLine;
    }
}
#pragma endregion State Methods


#pragma region Functional Methods
void updateLineValues()
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
}

void Spiral(int speed, int size)
{
  motors.setSpeeds(speed+size, speed*-1);
}
#pragma endregion Functional Methods


#pragma region Calculation Methods
int scale (int value, int maxOld, int maxNew)
{
  return (value / (maxOld / maxNew));
}
#pragma endregion


#pragma region Other Methods
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
  Serial.println(roboState);
  Serial.println("---------------------------------");
}
#pragma endregion Other Methods


#pragma region Default Methods
void setup() {
  Serial.begin(9600);
  reflectanceSensors.init();

  roboState = lookingForLine;
}

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
      Spiral(400, 0);
      break;
  }
  debug();
}
#pragma endregion Default Methods