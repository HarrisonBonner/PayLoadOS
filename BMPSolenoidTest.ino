/*
Joshua Hockey
10/1/23
Code blocks taken from BMPTest.ino and SolenoidTest.ino
Software SPI and Wiring implented using 
https://learn.adafruit.com/adafruit-bmp388-bmp390-bmp3xx/arduino
Relay Wiring for Solenoid implented using solenoid wiring diagram on the Github
To veiw what altitude is being read, ->serial monitor -> baud rate = 115200

This code activates the solenoid based on an altimeter reading.
*/

#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <AccelStepper.h>

#define RELAY_PIN_1 2
#define RELAY_PIN_2 3


//this is the value the altimiter compares against, this value works, but can be adjusted as needed
#define SEALEVELPRESSURE_HPA (1023) // Will need to be calibrated

// This is the altitude that the solenoid is triggered at
#define ALTITUDE_THRESHOLD 200.5 

//SPI pin definitions
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

//Relay pin for solenoid
#define motorRelay 8

Adafruit_BMP3XX bmp; // This must be here, no matter what communication standard is being used

void setup()
{
  
  Serial.begin(115200);//Baud rate for both BMP390 -> Arduino and Arduino -> USB

  //while we dont have a serial connection to the BMP390
  while (!Serial);
  Serial.println("Adafruit BMP388 / BMP390 test");

  //If there is a no good connection, Code wont work and itll not move forward
  if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
  }

  // Set up oversampling and filter initialization
  //I didnt add this, Adafruit seems to think that this is important
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  //Defines the Arduino pins 2,3, and 8 as outputs
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(motorRelay, OUTPUT);
}

void loop()
{
  // System Checking loop until we meet threshold

  //if the bmp fails to read, dont do anything
  //we only want good values for the altitude
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  
  //if the value is good, set it equal to a float
  float currentAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  //Prints the Current Altitude to the Serial Monitor for value verification
  Serial.print("Altitude: ");
  Serial.print(currentAltitude);
  Serial.println(" meters");


  // Check to see if it is the threshold, we technically need to hit it twice
  if (currentAltitude <= ALTITUDE_THRESHOLD)
  {
      digitalWrite(RELAY_PIN_1, HIGH); // Activate relay 1
      Serial.println("Relays activated!");//this is good for verification that the relays are actually supposed to be on
    
  }
  else{
    digitalWrite(RELAY_PIN_1, LOW); // Activate relay 1
    Serial.println("Relays deactivated!");
  }
  delay(200);//makes it so that the Serial Monitor isnt spammed
}

