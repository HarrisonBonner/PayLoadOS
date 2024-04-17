#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_MPU6050.h>
#include <RH_RF95.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define SEALEVELPRESSURE_HPA (1013.25)
#define MOTOR1_IN1 10
#define MOTOR1_IN2 11
#define MOTOR2_IN1 9
#define MOTOR2_IN2 6
#define altLED A2
#define messageLED A3
#define ejectLED A4
#define survivedLED A5

const int calibrationLoop = 100;
const int  groundAlt = 0; // PLUG IN GROUND ALT HERE
const float ALT_THRESHOLD = 300;   //Should be 300
const float ALT_THRESHOLD2 = 120;  // Should be 120
float targetAlt = 0;
float targetAlt2 = 0;
const int motorWindTime = 200;
const int sdAddress = 0x08;
const int master = 9;
int confCount = 0;

Adafruit_MPU6050 mpu;
Adafruit_BMP3XX bmp;
RH_RF95 rf95(RFM95_CS, RFM95_INT, hardware_spi);

void initBMP() {
  if (!bmp.begin_I2C()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1)
      ;
  }
  Serial.println("BMP found!");
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void initMPU() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
}

void initLoRa() {

  if (!rf95.init()) {
    Serial.println("LoRa initialization failed!");
    while (1)
      ;
  }
  rf95.setFrequency(868);
  Serial.println("LoRa receiver initialized!");
  //digitalWrite(RFM95_CS, HIGH);
}

void calibrateBMP() {
  int i = 0;
  while (i < calibrationLoop) {
    bmp.readAltitude(SEALEVELPRESSURE_HPA);
    i++;
  }
  targetAlt = ALT_THRESHOLD + groundAlt;
  targetAlt2 = ALT_THRESHOLD2 + groundAlt;
  String msg1 = "Target alt: ";
  msg1 += targetAlt;
  sendToSd(msg1);
  String msg2 = "Target alt2: ";
  msg2 += targetAlt2;
  sendToSd(msg2);
  Serial.print("Current alt: ");
  Serial.println(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.print("Target alt: ");
  Serial.println(targetAlt);
  Serial.println(targetAlt2);
  Serial.println("BMP calibrated!");
}

void sendToSd(String x) {
  Serial.print("Sending : ");
  Serial.println(x);
  Wire.beginTransmission(sdAddress);
  Wire.write(x.c_str());
  Wire.endTransmission();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.begin();
  Serial.begin(9600);
  //delay(200);
  //while (!Serial) { delay(100); }
  initLoRa();
  initMPU();
  initBMP();
  calibrateBMP();
}

void loop() {
  // put your main code here, to run repeatedly:

}
