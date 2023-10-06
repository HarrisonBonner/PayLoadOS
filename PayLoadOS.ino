#include <Adafruit_MPL3115A2.h>

#include <Adafruit_Sensor.h>
#include <Wire.h>
// #include "Adafruit_BMP3XX.h"
#include <SPI.h>

double ALTITUDE_THRESHOLD_ADD = 0.5; // Set your desired altitude threshold in meters
double ALTITUDE_THRESHOLD;
#define RELAY_PIN_2 7
#define motorRelay 6

#define SEALEVELPRESSURE_HPA 1013.25 // Will need to be calibrated

// #define BMP_SCK 13
// #define BMP_MISO 12
// #define BMP_MOSI 11
// #define BMP_CS 10

int count = 0;

// Adafruit_BMP3XX bmp; // I2C
//  Adafruit_BMP3XX bmp(BMP_CS); // hardware SPI
//  Adafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial Is Running");

  baro.setSeaPressure(SEALEVELPRESSURE_HPA);

  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(motorRelay, OUTPUT);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_2, LOW);
  digitalWrite(motorRelay, LOW);

  if (!baro.begin())
  {
    Serial.println("Could not find sensor. Check wiring.");
    while (1)
      ;
  }

  // Take a few readings to stablize the altimeter
  int testAltitude = baro.getAltitude();
  testAltitude = baro.getAltitude();
  testAltitude = baro.getAltitude();

  // Take 5 altitudes, average, then set as zero altitude
  double altitudeSum = 0;
  for (int i = 0; i < 5; i++)
  {
    altitudeSum += baro.getAltitude();
  }
  double avgAlt = altitudeSum / 5;
  ALTITUDE_THRESHOLD = avgAlt + ALTITUDE_THRESHOLD_ADD;

  Serial.print("avgAlt was: ");
  Serial.println(avgAlt);

  Serial.print("ALTITUDE_THRESHOLD is: ");
  Serial.println(ALTITUDE_THRESHOLD);
}

void loop()
{
  while(1){
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(1000);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(1000);
  }
  // System Checking loop until we meet threshold

  // Get current altitude



  float currentAltitude = baro.getAltitude();

  Serial.print("Altitude: ");
  Serial.print(currentAltitude);
  Serial.println(" meters");
  Serial.print("Count: ");
  Serial.println(count);
  // Check to see if it is the threshold, we technically need to hit it twice
  if (currentAltitude >= ALTITUDE_THRESHOLD && count > 3)
  {
    delay(1000);
    while (true)
    {

      currentAltitude = baro.getAltitude();
      Serial.println("In second loop");
      Serial.print("Altitude: ");
      Serial.print(currentAltitude);
      Serial.println(" meters");

      if (currentAltitude < ALTITUDE_THRESHOLD)
      {
        digitalWrite(motorRelay, HIGH);
        Serial.println("Motor relay activated");
        delay(3000);
        digitalWrite(RELAY_PIN_2, HIGH); // Activate relay 2
        Serial.println("Relays activated!");
        delay(3000);
        digitalWrite(RELAY_PIN_2, LOW); // Activate relay 2
        Serial.println("Solenoid turned off");
        while(1){}
      }
      delay(100);
    }
  }
  count++;

  delay(200);
}
