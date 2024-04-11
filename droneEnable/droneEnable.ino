#include <RH_RF95.h>
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
RH_RF95 rf95(RFM95_CS, RFM95_INT, hardware_spi);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //while(!Serial);
  initLoRa();
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

String getRadioMessage() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      buf[len] = '\0';
      Serial.println("Received message: ");
      //Serial.println((char*)buf);
      String msg = (char*)buf;

      if (msg.equals("Enable")) {
        Serial.println("Enable");
        return "Enable";
      }
    }
  }
  return "No Message";
}

void toggleSwitch() {
  digitalWrite(12, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(950);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(12, LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  if(getRadioMessage().equals("Enable")){
    toggleSwitch();
  }
  delay(100);
}
