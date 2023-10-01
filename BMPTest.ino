/***************************************************************************
  This is a library for the BMP3XX temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP388 Breakout
  ----> http://www.adafruit.com/products/3966

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
/*
Joshua Hockey
10/1/2023
This test code tests the Adafruit BMP390 over Software SPI
Most of the code was taken from:
https://learn.adafruit.com/adafruit-bmp388-bmp390-bmp3xx/arduino
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP3XX.h>
#include "Adafruit_BMP3XX.h"

//SPI Sensor definitions
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10


//this is the value the altimiter compares against, this value works, but can be adjusted as needed
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;// This must be here, no matter what communication standard is being used

void setup() {
  Serial.begin(115200);//SPI polling rate is 115200, over the default of 9600
  while (!Serial);
  Serial.println("Adafruit BMP388 / BMP390 test");

  if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
  }

  // Set up oversampling and filter initialization
  //I didnt add this, Adafruit seems to think that this is important
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void loop() {
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }


  //All of the values that the BMP390 can output
  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000);//Delay as to not spam the Serial Monitor
}

