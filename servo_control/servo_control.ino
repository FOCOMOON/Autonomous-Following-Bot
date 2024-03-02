/***************************************************
  HUSKYLENS An Easy-to-use AI Machine Vision Sensor
  <https://www.dfrobot.com/product-1922.html>

 ***************************************************
  This example shows the basic function of library for HUSKYLENS via Serial.

  Created 2020-03-13
  By [Angelo qiao](Angelo.qiao@dfrobot.com)

  GNU Lesser General Public License.
  See <http://www.gnu.org/licenses/> for details.
  All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
  1.Connection and Diagram can be found here
  <https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336#target_23>
  2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <Servo.h>

Servo myservo;
Servo myservo1;

int pos = 90;
int posi = 0;

HUSKYLENS huskylens;

void printResult(HUSKYLENSResult result);

void setup() {
  Serial.begin(9600);
  myservo.attach(2, 820, 2140);
  myservo1.attach(3, 820, 2140);
  while (!huskylens.begin(Serial))
  {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
}

void loop() {
  if (!huskylens.request()) {
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    myservo.write(90);
    myservo1.write(0);
  } else if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if (!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else
  {
    Serial.println(F("###########"));
    if (huskylens.available())
    {
      HUSKYLENSResult result = huskylens.read();
      printResult(result);

      int xAxis = result.xCenter;
      int yAxis = result.yCenter;
      int distance = result.height;


      Serial.print("xAxis");
      Serial.println(xAxis);
      Serial.print("yAxis");
      Serial.println(yAxis);
      Serial.print("distance");
      Serial.println(distance);

      if (xAxis < 133) {
        pos += 5.5;
        //delay(10);

      } else if (xAxis > 158) {
        pos -= 5.5;
        //delay(10);

      } else if (yAxis < 115) { //124
        posi += 5.5;
        //delay(10);


      } else if (yAxis > 135) {
        posi -= 5.5;
        //delay(10);

      } else {
        /*for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15 ms for the servo to reach the position
          }
          for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15 ms for the servo to reach the position
          }*/
      }


    } else {
      Serial.println("vision body undetected.");
      myservo.write(90);
      myservo1.write(0);
    }
  }
  if(pos >= 180){
    pos = 179;
  }
  if(posi >= 180){
    posi = 179;
  }
  if(pos <= 0){
    pos = 1;
  }
  if(posi <= 0){
    posi = 1;
  }
  myservo.write(pos);
  myservo1.write(posi);

}
void printResult(HUSKYLENSResult result) {
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  }
  else if (result.command == COMMAND_RETURN_ARROW) {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  }
  else {
    Serial.println("Object unknown!");

  }
}
