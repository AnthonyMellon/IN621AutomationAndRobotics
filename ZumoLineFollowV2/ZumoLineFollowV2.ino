#pragma region Includes
#include <Wire.h>
#include <ZumoShield.h>
#pragma endregion Includes

#define NUM_SENSORS 6
#define LINE_THRESHOLD 1100
#define MAX_SPEED 400
#define MAX_REFLECTANCE 6000

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors zMotors;
unsigned int sensor_values[NUM_SENSORS];
unsigned int leftValue, rightValue;

enum state {
  followingLine,
  lookingForLine,
  reOrientate
};
state roboState;


void setup() {
  Serial.begin(9600);
  reflectanceSensors.init();
}

void loop() {    

  reflectanceSensors.read(sensor_values);
  checkState();

  switch (roboState)
  {
    case followingLine:
        followLine();
        break;

    case lookingForLine:
        findLine();
        break;

    case reOrientate:
        spinToWin();
        break;
  }
}

void checkState()
{
    int totalReflectance = 0;

    roboState = lookingForLine;
    for(int i = 0; i < NUM_SENSORS; i++)
    {    
        totalReflectance += sensor_values[i];
        if(sensor_values[i] > LINE_THRESHOLD)
        {
            roboState = followingLine;
        }        
    } 

    Serial.println(totalReflectance);
    if(totalReflectance > MAX_REFLECTANCE*2 - 2000)
    {        
        roboState = reOrientate;
    }   
    else if(totalReflectance < 8000 && roboState != followingLine)
    {
        roboState = lookingForLine;
    }
}

void followLine()
{
    //This method will run one iteration for each loop of the main loop
    leftValue = 0;
    rightValue = 0;

    //Check if the number of sensors is even or odd for the sake of robustness
    if(NUM_SENSORS % 2 == 0)
    {
        for(int i = 0; i < NUM_SENSORS/2; i++)
        {
            leftValue += sensor_values[i];
            rightValue += sensor_values[i+NUM_SENSORS/2];
        }    
        leftValue /= (MAX_REFLECTANCE / MAX_SPEED);
        rightValue /= (MAX_REFLECTANCE / MAX_SPEED);
        
        zMotors.setSpeeds(MAX_SPEED - leftValue, MAX_SPEED - rightValue);

        Serial.print(leftValue);
        Serial.print(" | ");
        Serial.println(rightValue);
    }
    else
    {
        /* self destruct code */
    }
    


}

void findLine()
{
    //this method will run one iteration for each loop of the main loop
    zMotors.setSpeeds(100, -50);
}

void spinToWin()
{
    Serial.println("haha spinnnnnnnn");
    zMotors.setSpeeds(100, 0);
}