#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <AccelStepper.h>

#define ALTITUDE_THRESHOLD 1000  // Set your desired altitude threshold in meters
#define RELAY_PIN_1 2
#define RELAY_PIN_2 3
#define STEPPER_ENABLE_PIN 4
#define STEPPER_STEP_PIN 5
#define STEPPER_DIR_PIN 6
#define STEPS_PER_REV 200       // Set the steps per revolution for your stepper motor
#define SEALEVELPRESSURE_HPA 10

Adafruit_BMP3XX bmp;
AccelStepper stepper(AccelStepper::FULL4WIRE, STEPPER_STEP_PIN, STEPPER_DIR_PIN);

bool stepperActivated = false;
bool stepperDone = false;
int altitudeCount = 0;

int motorRev = 5;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin_I2C()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(STEPPER_ENABLE_PIN, OUTPUT);

  // Set the maximum speed and acceleration for the stepper motor
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);

  // Enable the stepper motor driver
  digitalWrite(STEPPER_ENABLE_PIN, LOW);
}

void loop() {
  //System Checking loop until we meet threshold
  if (!stepperActivated) {

    //Get current altitude
    float currentAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

    Serial.print("Altitude: ");
    Serial.print(currentAltitude);
    Serial.println(" meters");
    //Check to see if it is the threshold, we technically need to hit it twice
    if (currentAltitude == ALTITUDE_THRESHOLD) {
      altitudeCount++;

      //if it is the second time we hit the alittude we are go for ejection
      if (altitudeCount == 2) {
        stepperActivated = true; //Activate stepper and wait until it signals completion
        if (stepperDone) {
          digitalWrite(RELAY_PIN_1, HIGH); // Activate relay 1
          digitalWrite(RELAY_PIN_2, HIGH); // Activate relay 2
          Serial.println("Relays activated!");
        }
      }
    }
  }



  if (stepperActivated) { //Activiates on go for eject
    if (!stepperDone) {
      stepper.moveTo(STEPS_PER_REV * motorRev); // Move the motor x revolutions
      stepper.run(); //Sends signal to rotate to the stepper
      if (stepper.distanceToGo() == 0) {//Checks to see if we are done rotating
        stepperDone = true;
      }
    }
  }
}
