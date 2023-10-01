/*
Joshua Hockey
10/1/2023
Simple code that tests the DC motor
*/
#include <Wire.h>

//Sets the Solenoid Relays and Motor Relay to certain pin number
#define motorRelay 8

void setup()
{
  //Defines all of the pins as outputs
  //needed or else we cant write values to the pins
  pinMode(motorRelay, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(motorRelay, HIGH); // Turn on the Motor

  //LED is a troubleshooting step to make sure that the code is functioning properly
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    
  delay(1000);// amount of time the stuff is on


  digitalWrite(motorRelay, LOW); // Turn off Motor
  digitalWrite(LED_BUILTIN, LOW); //Turn off the on board LED

  delay(3000);// amount of time the stuff is off
}

