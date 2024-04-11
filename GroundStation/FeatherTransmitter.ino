#include <RH_RF95.h>
#include <LCD-I2C.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define windArmPin 11
#define ejectPin 13
#define windPin A1
#define unwindPin A0
#define enableDrone 12

RH_RF95 rf95(RFM95_CS, RFM95_INT);
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Serial began");
  if (!rf95.init()) {
    Serial.println("LoRa initialization failed!");
    while (1)
      ;
  }
  Serial.println("LoRa transmitter initialized!");
  rf95.setFrequency(868);
  rf95.setTxPower(20, false);

  lcd.begin();       //initialize the lcd
  lcd.display();
  lcd.backlight();  //open the backlight


  pinMode(ejectPin, INPUT);
  pinMode(windPin, INPUT);
  pinMode(unwindPin, INPUT);
  pinMode(windArmPin, INPUT);
  pinMode(enableDrone, INPUT);
}
void printLCDTop(String msg) {
  //clear top row of lcd
  lcd.setCursor(0, 0);  // move cursor to   (0, 0)
  lcd.print("                ");       // clear the top row of lcd
  lcd.setCursor(0, 0);  // move cursor to   (0, 0)
  lcd.print(msg);       // print message at (0, 0)
}
void printLCDBottom(String msg){
  //clear bottom row of lcd
  lcd.setCursor(0, 1);  // move cursor to   (0, 1)
  lcd.print("                ");       // clear the bottom row of lcd
  lcd.setCursor(0, 1);  // move cursor to   (0, 1)
  lcd.print(msg);       // print message at (0, 1)

}

String getRadioMessage(){
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)) {
      Serial.println((char*)buf);
      return (char*)buf;
    }
  }
  return "No MSG";

}
void loop() {


  if (digitalRead(windPin) && digitalRead(windArmPin)) {
    Serial.println("Sending Wind Signal!");
    printLCDTop("Wind");
    digitalWrite(2, HIGH);
    const char* message = "Wind";

    rf95.send((uint8_t*)message, strlen(message));
    rf95.waitPacketSent();
  }

  if (digitalRead(unwindPin) && digitalRead(windArmPin)) {
    Serial.println("Sending Unwind Signal!");
    printLCDTop("Unwind");

    digitalWrite(5, HIGH);

    const char* message = "Unwind";

    rf95.send((uint8_t*)message, strlen(message));
    rf95.waitPacketSent();
  }

  if (digitalRead(ejectPin)) {
    Serial.println("Sending Eject Signal!");
    printLCDTop("Eject");

    digitalWrite(5, HIGH);

    const char* message = "Eject";

    rf95.send((uint8_t*)message, strlen(message));
    rf95.waitPacketSent();
  }

  if (digitalRead(enableDrone)) {
    Serial.println("Sending Enable Signal!");
    printLCDTop("Enable");
    digitalWrite(5, HIGH);

    const char* message = "Enable";

    rf95.send((uint8_t*)message, strlen(message));
    rf95.waitPacketSent();
  }


  printLCDBottom(getRadioMessage());
  delay(100);

}
