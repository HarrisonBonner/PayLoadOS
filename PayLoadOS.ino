#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <AccelStepper.h>

#define ALTITUDE_THRESHOLD 1000 // Set your desired altitude threshold in meters
#define RELAY_PIN_1 2
#define RELAY_PIN_2 3
#define STEPPER_ENABLE_PIN 4
#define STEPPER_STEP_PIN 5
#define STEPPER_DIR_PIN 6
#define STEPS_PER_REV 200       // Set the steps per revolution for your stepper motor
#define SEALEVELPRESSURE_HPA 10 // Will need to be calibrated

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
#define motorRelay 8
Adafruit_BMP3XX bmp; // I2C
// Adafruit_BMP3XX bmp(BMP_CS); // hardware SPI
// dafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

int altitudeCount = 0;

void setup()
{
  Serial.begin(9600);
  if (!bmp.begin_I2C())
  {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1)
      ;
  }

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(motorRelay, OUTPUT);
}

void loop()
{
  // System Checking loop until we meet threshold

  // Get current altitude
  float currentAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  Serial.print("Altitude: ");
  Serial.print(currentAltitude);
  Serial.println(" meters");
  // Check to see if it is the threshold, we technically need to hit it twice
  if (currentAltitude == ALTITUDE_THRESHOLD)
  {
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
  }
}

