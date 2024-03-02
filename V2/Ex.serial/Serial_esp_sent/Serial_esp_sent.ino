//serial esp sent data
//https://www.hackster.io/RoboticaDIY/send-data-from-arduino-to-nodemcu-and-nodemcu-to-arduino-17d47a
#include <SoftwareSerial.h> //pin D4
int n = 1;
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}
void loop()
{
  Serial1.println(1+n);
  delay(1000);
}
