const int stepPin = 2; //X.STEP
const int dirPin = 5; // X.DIR

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

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
Data_Package data;

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;



int x = 0;
int SMSpeed = 100;
int checking = 0;
int previous = 0;
int long newval = 0;
int motorSpeed = 1;  

void setup() {
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setRetries(0, 15);
  radio.setPALevel(RF24_PA_HIGH);
}

void loop() {
  radio.startListening();
  if ( radio.available() )
  {

    radio.read(&data, sizeof(Data_Package));
    lastReceiveTime = millis();
  }
  currentTime = millis();
  if (currentTime - lastReceiveTime > 1000) {
    resetData();
  }
  show_value();
  control();
  radio.stopListening();
  radio.write(&data, sizeof(Data_Package));

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
void control() {
  int stepspeed = map(data.pot1, 0, 240, 1, 1000);
  
  if ((data.joy2_Y > 120)  &&   (data.joy2_Y < 180)) {
   
  }
  if (data.joy2_Y > 250 )
  {
    motorSpeed = (data.joy2_Y/15 + 5); 
    counterclockwise();
  }

  if (data.joy2_Y < 80 )
  {
    motorSpeed = ((1024-data.joy2_Y)/15 + 5);
    clockwise();
  }
}
void resetData() {
  // Set initial default values
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

void counterclockwise () {
  digitalWrite(dirPin, LOW);
  x = x - 1;

  digitalWrite(stepPin, HIGH);
  delayMicroseconds(SMSpeed);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(SMSpeed);
}

void clockwise() {
  digitalWrite(dirPin, HIGH);
  x = x + 1;
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(SMSpeed);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(SMSpeed);
}
