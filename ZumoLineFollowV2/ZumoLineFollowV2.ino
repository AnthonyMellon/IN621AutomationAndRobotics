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
  onLine, //on the line
  justEnteredAether, //only just fell onto the whitespace, still hope to get back on the line
  lostInAether, //completely lost in the whitespace
  inVoid, //on the blackspace
  unknown
};
state roboState;


void setup() {
  Serial.begin(9600);
  reflectanceSensors.init();
}

void loop() {    

  reflectanceSensors.read(sensor_values);
  updateState(checkState());

  switch (roboState)
  {
    case onLine:
        followLine();
        break;

    case justEnteredAether:
        escapeAether();
        break;

    case inVoid:
        escapeVoid();
        break;
    
    case unknown:
        Serial.println("Unknown state");
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

    for(int i = 0; i < NUM_SENSORS; i++) //If any sensor picks up the line, robot is assumed to be on earth
    {    
        totalReflectance += sensor_values[i];

        if(sensor_values[i] > LINE_THRESHOLD)
        {
            tempState = onLine;
        }        
    } 
    if(tempState != onLine) //If the robot is not considered to be on earth after all that, it must be in the aether
    {
        tempState = justEnteredAether;
    }    
    else if(totalReflectance > MAX_REFLECTANCE*2 - 1000) //If all the sensors are picking up the line (minus some slack) then the robot must be in the void
    {     
        //Serial.println("Going into the void");
        tempState = inVoid;
    }   
    

    return tempState;
}

void followLine()
{
    Serial.println("Safe on earth");
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

        // Serial.print(leftValue);
        // Serial.print(" | ");
        // Serial.println(rightValue);
    }
    else
    {
        /* self destruct code */
    }
    


}

void escapeAether() //Stay in the method until out of aether
{
    Serial.println("Escaping Aether");
    zMotors.setSpeeds(400, 100);
    updateState(checkState());


    #pragma region Problem code that makes me want to die
    //Start with a full rotation. This is the justEnteredAether state
    // state nextState = unknown;
    // zMotors.setSpeeds(100, -100);
    // do
    // {
    //     delay(10);

    //     //Stop the robot from continuiosly re-entering the justEnteredAether state
    //     nextState = checkState();
    //     Serial.println(nextState);
    //     if(nextState != justEnteredAether)
    //     {
    //         Serial.println("Leaving the aether");
    //         updateState(nextState);
    //     }
    //     if(/*full rotation achieved*/ true == false)
    //     {
    //         //rotate 45(?) degrees to the right
    //         roboState == lostInAether;
    //     }

    // } while (roboState == justEnteredAether);

    // zMotors.setSpeeds(0,0);

    // nextState = unknown;
    // do
    // {
    //     zMotors.setLeftSpeed(MAX_SPEED);  
        
    //     for(int i = 0; i < MAX_SPEED; i++)
    //     {
    //         zMotors.setRightSpeed(i);
    //         for(int d = 0; d < 10; d++)
    //         {                
    //             nextState = checkState();
    //             if(nextState != justEnteredAether)
    //             {
    //                 updateState(nextState);
    //                 zMotors.setSpeeds(0, 0);
    //                 d = 11;
    //             }
    //             delay(10);
    //         }
    //     }

    // } while (roboState == lostInAether);
    #pragma endregion
    

    //Continue with a spiral that only ends once a line is found. This is the lostInAether state
}

void escapeVoid()
{
    Serial.println("Escaping void");
    zMotors.setSpeeds(100, 0);
    updateState(checkState());
}