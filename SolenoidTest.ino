/*
Joshua Hockey
10/1/2023
Simple test to make sure that the relays that turn on the solenoids work
*/
#include <Wire.h>

#define RELAY_PIN_1 2
#define RELAY_PIN_2 3

int altitudeCount = 0;

void setup()
{
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    
    digitalWrite(RELAY_PIN_1, HIGH); // Activate relay 1
    digitalWrite(RELAY_PIN_2, HIGH); // Activate relay 2

    //LED is a troubleshooting step to make sure that the code is functioning properly
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    
    delay(1000);

    

    digitalWrite(RELAY_PIN_1, LOW); // Deactivate relay 1
    digitalWrite(RELAY_PIN_2, LOW); // Deactivate relay 2

    digitalWrite(LED_BUILTIN, LOW);

    delay(3000);
}

