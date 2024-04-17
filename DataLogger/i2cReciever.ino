#include <Wire.h>
#include <SD.h>
const int chipSelect = 4;

void setup() {
  Wire.begin(0x08);           // join i2c bus with address #8
  Wire.onReceive(onReceive);  // register event
  Serial.begin(9600);
  //while(!Serial);
  digitalWrite(LED_BUILTIN, HIGH);
  initSD();
}

void initSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");
}

void loop() {
  delay(100);
}


void onReceive(int num) {
  String x = "";
  while (Wire.available()) {
    x += (char)Wire.read();
    Serial.println("recieved");
  }
  writeToSD(x);
  Serial.println(x);
}

void writeToSD(String msg) {
  File dataFile = SD.open("data.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(msg);
    dataFile.close();
    Serial.println("Done.");
  } else {
    // if the file isn't open, pop up an error:
    Serial.println("Error opening file.");
  }
}
