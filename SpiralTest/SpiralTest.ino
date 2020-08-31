#pragma region Includes
#include <Wire.h>
#include <ZumoShield.h>
#pragma endregion Includes

ZumoMotors zMotors;




void setup() {
    Serial.begin(9600);

    zMotors.setLeftSpeed(400);  
    for(int i = 0; i < 400; i++)
    {
        Serial.println(i);
        zMotors.setRightSpeed(i);
        delay(100);
    }
}

void loop() {  

}
