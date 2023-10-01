/*
Joshua Hockey 
10/1/2023
Simple code that activates both the solenoids and the motor at the same time
Total current draw was 4.5 Amp, which is less than the 10 out batteris can output
*/
#include <Wire.h>

//Sets the Solenoid Relays and Motor Relay to certain pin number
//This is mainly for readability
#define RELAY_PIN_1 2
#define RELAY_PIN_2 3
#define motorRelay 8

int altitudeCount = 0;

void setup()
{
  //Defines all of the pins as outputs
  //needed or else we cant write values to the pins
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(motorRelay, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    
    digitalWrite(RELAY_PIN_1, HIGH); // Activate relay 1
    digitalWrite(RELAY_PIN_2, HIGH); // Activate relay 2
    digitalWrite(motorRelay, HIGH); // Turn on the Motor

    //LED is a troubleshooting step to make sure that the code is functioning properly
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    
    delay(1000);// amount of time the stuff is on



    digitalWrite(RELAY_PIN_1, LOW); // Deactivate relay 1
    digitalWrite(RELAY_PIN_2, LOW); // Deactivate relay 2
    digitalWrite(motorRelay, LOW); // Turn off Motor
    digitalWrite(LED_BUILTIN, LOW); //Turn off the on board LED

    delay(3000);// amount of time the stuff is off
    
}

