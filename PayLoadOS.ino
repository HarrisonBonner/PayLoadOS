#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Adafruit_BMP3XX.h"
#include <SPI.h>

int ALTITUDE_THRESHOLD_ADD = 130; // Set your desired altitude threshold in meters
int ALTITUDE_THRESHOLD;
#define RELAY_PIN_1 9
#define RELAY_PIN_2 8
#define motorRelay 7

#define SEALEVELPRESSURE_HPA 1013.25 // Will need to be calibrated

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
int count = 0;

Adafruit_BMP3XX bmp; // I2C
// Adafruit_BMP3XX bmp(BMP_CS); // hardware SPI
// Adafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

void setup()
{
  Serial.begin(9600);
  if (!bmp.begin_SPI(BMP_CS))
  {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1)
      ;
  }

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(motorRelay, OUTPUT);
  
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);
  digitalWrite(motorRelay, LOW);

    if (!bmp.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  //Take a few readings to stablize the altimeter 
  int testAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  testAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  testAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  //Take 5 altitudes, average, then set as zero altitude
  int altitudeSum = 0;
  for(int i = 0; i < 5; i++){
    altitudeSum += bmp.readAltitude(SEALEVELPRESSURE_HPA);
  }
  int avgAlt = altitudeSum / 5;
  ALTITUDE_THRESHOLD = avgAlt + ALTITUDE_THRESHOLD_ADD;

  Serial.print("avgAlt was: ");
  Serial.println(avgAlt);

  Serial.print("ALTITUDE_THRESHOLD is: ");
  Serial.println(ALTITUDE_THRESHOLD);

}

void loop()
{
  digitalWrite(8, HIGH);
  delay(1000);
  digitalWrite(8,LOW);
  delay(1000);
  // System Checking loop until we meet threshold

  // Get current altitude

  // if (!bmp.performReading())
  // {
  //   Serial.println("Failed to perform reading :(");
  //   return;
  // }

  // float currentAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  // Serial.print("Altitude: ");
  // Serial.print(currentAltitude);
  // Serial.println(" meters");
  // Serial.print("Count: ");
  // Serial.println(count);
  // // Check to see if it is the threshold, we technically need to hit it twice
  // if (currentAltitude >= ALTITUDE_THRESHOLD && count > 3)
  // {
  //   delay(1000);
  //   while (true)
  //   {

  //     currentAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  //     Serial.println("In second loop");
  //     Serial.print("Altitude: ");
  //     Serial.print(currentAltitude);
  //     Serial.println(" meters");

  //     if (currentAltitude < ALTITUDE_THRESHOLD)
  //     {
  //       digitalWrite(motorRelay, HIGH);
  //       Serial.println("Motor relay activated");
  //       delay(3000);
  //       digitalWrite(RELAY_PIN_1, HIGH); // Activate relay 1
  //       digitalWrite(RELAY_PIN_2, HIGH); // Activate relay 2
  //       Serial.println("Relays activated!");
  //       delay(3000);
  //       digitalWrite(RELAY_PIN_1, LOW); // Activate relay 1
  //       digitalWrite(RELAY_PIN_2, LOW); // Activate relay 2
  //       break;
  //     }
  //     delay(100);
  //   }
  // }
  // count++;

  // delay(200);
}
