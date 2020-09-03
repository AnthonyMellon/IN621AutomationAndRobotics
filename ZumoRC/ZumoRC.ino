#pragma region Includes
#include <Wire.h>
#include <ZumoShield.h>
#pragma endregion Includes

#define NUM_SENSORS 6
#define MAX_SPEED 400


String input;
String output;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors zMotors;
unsigned int sensor_values[NUM_SENSORS];


void setup() {
  Serial.begin(9600);
    Serial.setTimeout(10);
  reflectanceSensors.init();
}

void loop() {    
    
    input = Serial.readString();
    if(input == "W")
    {
        zMotors.setSpeeds(100, 100);
    }
    else if (input == "S")
    {
        zMotors.setSpeeds(-100, -100);
    }
    else if (input == "D")
    {
        zMotors.setSpeeds(200, -200);
    }
    else if (input == "A")
    {
        zMotors.setSpeeds(-200, 200);
    }
    else if (input == "F")
    {
        zMotors.setSpeeds(0, 0);
    }

    reflectanceSensors.read(sensor_values);
    output = "";
    for(int i = 0; i < NUM_SENSORS; i++)
    {
        output += i;
        output += ":";
        output += sensor_values[i];
        output += ";";
    }
    Serial.println(output);
}
