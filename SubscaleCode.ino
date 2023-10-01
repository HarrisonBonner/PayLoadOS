/*
Joshua Hockey
Harrison Bonner
10/1/2023

Once the BMP390 detects a certain altitude, the motor will turn on
ejecting the nosecone, and once thats done, the solenoids will eject the 
payload from the body of the rocket.
*/
#include <Wire.h>
#include <Adafruit_BMP3XX.h>


//this is the value the altimiter compares against, this value works, but can be adjusted as needed
#define SEALEVELPRESSURE_HPA (1023) // Will need to be calibrated

// This is the altitude that the solenoid is triggered at
#define ALTITUDE_THRESHOLD 200.5 

//Pin definitions for SPI interface
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

//Sets the Solenoid Relays and Motor Relay to certain pin number
//This is mainly for readability
#define RELAY_PIN_1 2
#define RELAY_PIN_2 3
#define motorRelay 8


Adafruit_BMP3XX bmp; // This must be here, no matter what communication standard is being used

int altitudeCount = 0;// variable to make sure it doesnt deploy when going up, only when going down

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
  
  f//if the value is good, set it equal to a float
  float currentAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  //Prints the Current Altitude to the Serial Monitor for value verification
  Serial.print("Altitude: ");
  Serial.print(currentAltitude);
  Serial.println(" meters");

  
  
    altitudeCount++;

    // if it is the second time we hit the alittude we are go for ejection
    if (altitudeCount == 2)
    {
      digitalWrite(motorRelay, HIGH);
      delay(100);
      digitalWrite(RELAY_PIN_1, HIGH); // Activate relay 1
      digitalWrite(RELAY_PIN_2, HIGH); // Activate relay 2
      Serial.println("Relays activated!");
    }
  // Check to see if it is the threshold, we technically need to hit it twice
  if (currentAltitude <= ALTITUDE_THRESHOLD)
  {
    
    altitudeCount++;

    //delays the motors/solonoids the first time the rocket meets the target altitude
    //This is so the rocket doesnt eject while going up
    //The delay makes is so it doesnt double measure the same altitude
    if (altitudeCount == 1){
      delay(4000);
    }
    else if (altitudeCount >= 2)
    {
      digitalWrite(motorRelay, HIGH);
      delay(100);//this value will be changed to reflect how long it takes to eject the nose cone from the payload
      digitalWrite(RELAY_PIN_1, HIGH); // Activate relay 1
      digitalWrite(RELAY_PIN_2, HIGH); // Activate relay 2
      Serial.println("Relays activated!");
    }
  }
  //delay(200);//makes it so that the Serial Monitor isnt spammed
}

