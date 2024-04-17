
#include <Wire.h>
#include <SPI.h>
//#include <Adafruit_Sensor.h>
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
    while (1)
      ;
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void initMPU() {
  if (!mpu.begin()) {
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
}

void initLoRa() {

  if (!rf95.init()) {
    while (1)
      ;
  }
  rf95.setFrequency(868);
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
  //sendToSd(msg1);
  String msg2 = "Target alt2: ";
  msg2 += targetAlt2;
  //sendToSd(msg2);
  Serial.print("Current alt: ");
  Serial.println(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.print("Target alt: ");
  Serial.println(targetAlt);
  Serial.println(targetAlt2);
  Serial.println("BMP calibrated!");
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

// String doubleToString(double x){
//   return String(x, 3);
// }

// void sendToSd(String x) {
//   Wire.beginTransmission(sdAddress);
//   Wire.write(x.c_str());
//   Wire.endTransmission();
// }

String getRadioMessage() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      buf[len] = '\0';
      String msg = (char*)buf;

      if (msg.equals("Eject")) {
        Serial.println("Eject");
        return "Eject";
      } else if (msg.equals("Wind")) {
        Serial.println("Wind");
        return "Wind";
      } else if (msg.equals("Unwind")) {
        Serial.println("Unwind");
        return "Unwind";
      }
    }
  }
  return "No Message";
}

void sendRadioMessage(String msg){
  rf95.send((uint8_t*)msg.c_str(), strlen(msg.c_str()));
  rf95.waitPacketSent();
}

bool checkRadioEject() {
  if (getRadioMessage().equals("Eject")) {
    //sendToSd("Eject signal recieved");
    digitalWrite(messageLED, HIGH);
    return true;
  }
  return false;
}

bool checkRadioWind() {
  if (getRadioMessage().equals("Wind")) {
    return true;
  }
  return false;
}

bool checkRadioUnwind() {
  if (getRadioMessage().equals("Unwind")) {
    return true;
  }
  return false;
}

void windMotors() {
  digitalWrite(MOTOR2_IN1, HIGH);
  delay(motorWindTime);
  digitalWrite(MOTOR2_IN1, LOW);
}

void radioUnwindMotors() {
  digitalWrite(MOTOR2_IN2, HIGH);
  delay(motorWindTime);
  digitalWrite(MOTOR2_IN2, LOW);
}

void ejectMotors() {
  digitalWrite(MOTOR2_IN2, HIGH);
  while (bmp.readAltitude(SEALEVELPRESSURE_HPA) > targetAlt2 && !checkRadioEject()) { delay(100); }
  digitalWrite(MOTOR1_IN2, HIGH);
}

bool ifSurvive() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  double accel = sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2));
  if (accel >= 98.1) {
    return true;
  }
  return false;
}

void loop() {

  if (checkRadioUnwind()) {
    radioUnwindMotors();
  }
  delay(50);
  if (checkRadioWind()) {
    windMotors();
  }
  delay(50);


  float alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Current alt: ");
  Serial.println(alt);
  if (alt > targetAlt) {
    digitalWrite(altLED, HIGH);
    while (1) {
      confCount++;
      alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
      if (alt < targetAlt && confCount > 5) {
        digitalWrite(ejectLED, HIGH);
        ejectMotors();
        while (1) {
          if (ifSurvive()) {
            digitalWrite(survivedLED, HIGH);
          }
        }
      }
    }
  }
}
