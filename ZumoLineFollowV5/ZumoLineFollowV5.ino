#include <Wire.h>
#include <ZumoShield.h>
#include <Adafruit_NeoPixel.h>

#define NEON_PIN 3
#define NLIGHTS 8
#define NEON_SPEED 5
#define NUM_SENSORS 6
#define SINGLE_ON_LINE_THRESHOLD 1200
#define MAX_SPEED 200
#define SPIRAL_DELAY 50
#define MAX_REFLECTANCE 2000

uint8_t numSensorsOnLine;
uint64_t spiralTime;


unsigned int sensorValues[NUM_SENSORS];
unsigned int leftSensorValue;
unsigned int rightSensorValue;
unsigned int leftSpeed;
unsigned int rightSpeed;

//neon lights
int neon_red;
int neon_green;
int neon_blue;
int neon_hue;
float neon_repeats;
uint32_t neon_rgbColour;
Adafruit_NeoPixel neonKit(NLIGHTS, NEON_PIN, NEO_GRB + NEO_KHZ800);

enum direction
{
    DLeft,
    DRight,
};
direction turnPreference;

enum state
{
    SCenterOnLine,
    SPrepareToTurn,
    SDriveStraight,
    SSpiral,    
};
state roboState;

ZumoMotors zMotors;
ZumoReflectanceSensorArray zReflectorSensors;

void setup()
{
    zReflectorSensors.init();

    neon_blue = 0;
    neon_green = 0;
    neon_red = 0;
    neon_hue = 0;
    neon_repeats = 0.25;

    neonKit.begin();
}

void loop()
{
    checkSensors();
    checkState();
    updateNeons();

    switch (roboState)
    {
        case SCenterOnLine:
            centerOnLine();
            break;
        
        case SPrepareToTurn:
            PrepareToTurn();
            break;

        case SDriveStraight:
            DriveStraight();
            break;

        case SSpiral:
            Spiral();
            break;

        default:
            break;
    }
}

void checkState()
{
    if (numSensorsOnLine == NUM_SENSORS)    
    {
        roboState = SDriveStraight;
    }
    else if(numSensorsOnLine == NUM_SENSORS - 1)
    {
        roboState = SPrepareToTurn;
    }   
    else if (numSensorsOnLine == 0)
    {
        roboState = SSpiral;
    }
    else
    {
        roboState = SCenterOnLine;
    }


    //reset the spiral timer when the robot falls out of the spiral
    if(roboState != SSpiral)
    {
        spiralTime = 0;
    }
    
}

void checkSensors()
{
    numSensorsOnLine = 0;
    zReflectorSensors.read(sensorValues);
    for(int i = 0; i < NUM_SENSORS; i++)
    {
        if(sensorValues[i] > SINGLE_ON_LINE_THRESHOLD)
        {
            numSensorsOnLine++;
        }
    }
}

void centerOnLine()
{
    leftSensorValue = 0;
    rightSensorValue = 0;

    //Get the total 'amount' of line on the left of the robot
    for(int i = 0; i < NUM_SENSORS/2; i++)
    {
        leftSensorValue += sensorValues[i];
    }

    //Get the total 'amount' of line on the right of the robot
    for(int i = NUM_SENSORS/2; i < NUM_SENSORS; i++)
    {
        rightSensorValue += sensorValues[i];
    }

    //scale the sensor values down to appropriate speeds
    rightSpeed = leftSensorValue / (MAX_REFLECTANCE / MAX_SPEED);
    leftSpeed = rightSensorValue / (MAX_REFLECTANCE / MAX_SPEED);

    zMotors.setSpeeds(leftSpeed, rightSpeed);    

}

void PrepareToTurn() //updating the turn preference then driving foward
{
    //Update turn preference    
    if(sensorValues[0] < SINGLE_ON_LINE_THRESHOLD) //If the far left sensor is on the line
    {
        turnPreference = DLeft;
    }
    else if(sensorValues[NUM_SENSORS] < SINGLE_ON_LINE_THRESHOLD) //If the far right sensor is on the line
    {
        turnPreference = DRight;
    }

    DriveStraight();
}

void DriveStraight() //drive fowards
{
    zMotors.setSpeeds(MAX_SPEED, MAX_SPEED);
}

void Spiral() //spiral outwards
{
    //decide on weather to turn left or right    
    switch (turnPreference)
    {
        case DLeft:
            zMotors.setSpeeds(MAX_SPEED, (MAX_SPEED*-1) + spiralTime);
            break;

        case DRight:
            zMotors.setSpeeds((MAX_SPEED*-1) + spiralTime, MAX_SPEED);
            break;
        
        default:
            break;
    }

    //increase the diameter of the spiral
        spiralTime++;
        if(spiralTime >= MAX_SPEED)
        {
            spiralTime = MAX_SPEED;
        }
        delay(SPIRAL_DELAY);
}

void updateNeons()
{
    neon_rgbColour = neonKit.ColorHSV(neon_hue);
    for(int i = 0; i < NLIGHTS; i++)
    {        
        neonKit.setPixelColor(i, neon_rgbColour);         
    } 
    neonKit.show();   
    neon_hue += NEON_SPEED; 
}