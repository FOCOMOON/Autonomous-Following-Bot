#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

bool lastState = -1;

#define joybtn1 0  // Joystick button 1
#define joybtn2 1  // Joystick button 2
#define tgl1 7   // Toggle switch 1
#define tgl2 4   // Toggle switch 1
#define btn1 8   // Button 1
#define btn2 9   // Button 2

RF24 radio(5, 6);   // nRF24L01 (CE, CSN)
const byte addresses[][6] = {"00001", "00002"};

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte joy1_X;
  byte joy1_Y;
  byte j1Button;
  byte joy2_X;
  byte joy2_Y;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;

};

Data_Package data; //Create a variable with the above structure
int check = 2;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  finger.begin(57600);
  if (finger.verifyPassword()) {

    Serial.println("Found fingerprint sensor!");

  } else {

    Serial.println("Did not find fingerprint sensor :(");

    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();

  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");

  Serial.println("Waiting for valid finger...");

  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Put your finger");
  // radio communication

  pinMode(joybtn1, INPUT_PULLUP);
  pinMode(joybtn2, INPUT_PULLUP);
  pinMode(tgl1, INPUT_PULLUP);
  pinMode(tgl2, INPUT_PULLUP);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  // initial default values
  data.joy1_X = 127;
  data.joy1_Y = 127;
  data.joy2_X = 127;
  data.joy2_Y = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
}


void loop() {
  while (check == 1) {
    int id;
    id = getFingerprintIDez();

    if ( id == -1 ) {

      if ( lastState == 0) {
        lastState = -1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access denied");
        delay(1000);
        lcd.setCursor(0, 0);
        lcd.print("Put your finger");
      }
    }

    else if ( id != -1) {
      Serial.print(finger.fingerID);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Authorize access");
      check = 2;
      delay(1000);
    }
  }

  if (check == 2) {
    sent_data();
  }
}

void sent_data() {
  // Read all analog inputs and map them to one Byte value
  data.joy1_X = map(analogRead(A1), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.joy1_Y = map(analogRead(A0), 0, 1023, 0, 255);
  data.joy2_X = map(analogRead(A2), 0, 1023, 0, 255);
  data.joy2_Y = map(analogRead(A3), 0, 1023, 0, 255);
  data.pot1 = map(analogRead(A7), 0, 1023, 0, 255);
  data.pot2 = map(analogRead(A6), 0, 1023, 0, 255);
  // Read all digital inputs
  data.j1Button = digitalRead(joybtn1);
  data.j2Button = digitalRead(joybtn2);
  data.tSwitch2 = digitalRead(tgl2);
  data.tSwitch1 = digitalRead(tgl1);
  data.button1 = digitalRead(btn1);
  data.button2 = digitalRead(btn2);
  show_value();
  radio.stopListening();
  radio.write(&data, sizeof(Data_Package));
  radio.startListening();
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));
    lcd.setCursor(0, 0);
    lcd.print("Robot Connected");
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    /*if (data.button2 == 0 ) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Safety Lock");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Put your finger");
      check = 1;
    }*/
  } else {
    lcd.setCursor(0, 0);
    lcd.print("  Searching...  ");
    lcd.setCursor(0, 1);
    lcd.print("Robot Disconnected");
  }
}

void show_value () {
  Serial.print("joy1_X: ");
  Serial.print(data.joy1_X);
  Serial.print("; joy1_Y: ");
  Serial.print(data.joy1_Y);
  Serial.print("; joy2_X: ");
  Serial.print(data.joy2_X);
  Serial.print("; joy2_Y: ");
  Serial.print(data.joy2_Y);
  Serial.print("; button1: ");
  Serial.print(data.button1);
  Serial.print("; button2: ");
  Serial.print(data.button2);
  Serial.print("; j1Button: ");
  Serial.print(data.j1Button);
  Serial.print("; j2Button: ");
  Serial.print(data.j2Button);
  Serial.print("; tSwitch1: ");
  Serial.print(data.tSwitch1);
  Serial.print("; tSwitch2: ");
  Serial.print(data.tSwitch2);
  Serial.print("; pot1: ");
  Serial.print(data.pot1);
  Serial.print("; pot2: ");
  Serial.println(data.pot2);
}

uint8_t getFingerprintID() {

  uint8_t p = finger.getImage();

  switch (p) {

    case FINGERPRINT_OK:

      Serial.println("Image taken");

      break;

    case FINGERPRINT_NOFINGER:

      Serial.println("No finger detected");

      return p;

    case FINGERPRINT_PACKETRECIEVEERR:

      Serial.println("Communication error");

      return p;

    case FINGERPRINT_IMAGEFAIL:

      Serial.println("Imaging error");

      return p;

    default:

      Serial.println("Unknown error");

      return p;

  }

  // OK success!

  p = finger.image2Tz();

  switch (p) {

    case FINGERPRINT_OK:

      Serial.println("Image converted");

      break;

    case FINGERPRINT_IMAGEMESS:

      Serial.println("Image too messy");

      return p;

    case FINGERPRINT_PACKETRECIEVEERR:

      Serial.println("Communication error");

      return p;

    case FINGERPRINT_FEATUREFAIL:

      Serial.println("Could not find fingerprint features");

      return p;

    case FINGERPRINT_INVALIDIMAGE:

      Serial.println("Could not find fingerprint features");

      return p;

    default:

      Serial.println("Unknown error");

      return p;

  }
  // OK converted!

  p = finger.fingerFastSearch();

  if (p == FINGERPRINT_OK) {

    Serial.println("Found a print match!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {

    Serial.println("Communication error");

    return p;

  } else if (p == FINGERPRINT_NOTFOUND) {

    Serial.println("Did not find a match");

    return p;

  } else {

    Serial.println("Unknown error");

    return p;

  }

  // found a match!

  Serial.print("Found ID #"); Serial.print(finger.fingerID);

  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;

}

// returns -1 if failed, otherwise returns ID #

int getFingerprintIDez() {

  uint8_t p = finger.getImage();

  Serial.println(p);

  if ( p == 0 ) {

    lastState = 0;

  }

  if (p != FINGERPRINT_OK)  return -1;



  p = finger.image2Tz();

  if (p != FINGERPRINT_OK)  return -1;



  p = finger.fingerFastSearch();

  if (p != FINGERPRINT_OK)  return -1;



  lastState = 1;

  // found a match!

  Serial.print("Found ID #"); Serial.print(finger.fingerID);

  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;

}
