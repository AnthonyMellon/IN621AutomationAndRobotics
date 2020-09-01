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
ZumoBuzzer zBuzzer;
unsigned int sensor_values[NUM_SENSORS];
unsigned int leftValue, rightValue;

enum state {
    unknown,
    onLine,
    prepareToTurn,
    turn,
    inVoid
  
};
state roboState;

enum direction {
    none,
    left,
    right,
    foward,
    back
};
direction turnPreference;


void setup() {
  Serial.begin(9600);
  reflectanceSensors.init();
 
}

void loop() {    

  reflectanceSensors.read(sensor_values);
  updateState(checkState());
  turnPreference = checkTurnPreference();
  printDebug();

  switch (roboState)
  {
    case onLine:
        followLine();
        break;

    case prepareToTurn:
        DriveFoward(100);
        break;

    case turn:
        turnBot();
        break;
    
    case inVoid:
        escapeVoid();
        break;

    case unknown:
        zMotors.setSpeeds(-100, -100);
        break;
  }
  
}

void updateState(state newState)
{
    roboState = newState;
}

state checkState()
{    
    state tempState = unknown;

    int totalReflectance = 0;

    for(int i = 0; i < NUM_SENSORS; i++) //If any sensor picks up the line, robot is assumed to be on line
    {    
        totalReflectance += sensor_values[i];

        if(sensor_values[i] > LINE_THRESHOLD)
        {
            tempState = onLine;
        }        
    } 
    if(totalReflectance > (MAX_REFLECTANCE*2) - 1)
    {
        tempState = inVoid;
    }

    //Preparing to turn as long the the bost is still able to follow the line 
    if(turnPreference != none && tempState == onLine)
    {
        tempState = prepareToTurn;
    }

    //Once the robot can no longer follow the line, start turning
    else if(turnPreference != none && tempState != onLine)
    {
        tempState = turn;
    }

    //Serial.println(tempState);
    return tempState;
}

direction checkTurnPreference()
{
    direction tempPreference = turnPreference;

    //only update turn preference if a turn preference does not already exist, and the robot is not in the void
    if(roboState != inVoid && tempPreference == none)
    {
        //If the far left sensor is off the line, and the far right sensor is on the line
        if(sensor_values[0] < LINE_THRESHOLD && sensor_values [NUM_SENSORS] > LINE_THRESHOLD)
        {
            tempPreference = right;
        }

        //If the far right sensor is off the line, and the far left sensor is on the line
        if (sensor_values[NUM_SENSORS] < LINE_THRESHOLD && sensor_values [0] > LINE_THRESHOLD)
        {
            tempPreference = left;
        } 
    }

    //Serial.println(turnPreference);
    return tempPreference;
}

void followLine()
{
    leftValue = 0;
    rightValue = 0;
    turnPreference = none;

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

        // Serial.print(leftValue);
        // Serial.print(" | ");
        // Serial.println(rightValue);
    }
    else
    {
        /* self destruct code */
    }
}

void DriveFoward(int speed)
{
    zMotors.setSpeeds(speed, speed);
}

void turnBot()
{
    switch (turnPreference)
    {
        case left:
            zMotors.setSpeeds(300, 100);
            break;
        
        case right:
            zMotors.setSpeeds(100, 300);
            break;

        default:
            zMotors.setSpeeds(0, 0);
            break;
    }
}

void printDebug()
{
    Serial.print("RoboState: ");
    Serial.println(roboState);

    Serial.print("turnPreference: ");
    Serial.println(turnPreference);

    Serial.println("----------------------------------------------");
}

void escapeVoid()
{
    zMotors.setSpeeds(100, 100);
}