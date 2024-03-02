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

#define N1 4 
#define N2 5 
#define N3 12 
#define N4 13

int speedPinA = 9;
int speedPinB = 4;

int speedA = 150;
int speedB = 180;

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void printResult(HUSKYLENSResult result);

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  while (!huskylens.begin(mySerial))
  {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }

  pinMode (N1, OUTPUT);
  pinMode (N2, OUTPUT);
  pinMode (N3, OUTPUT);
  pinMode (N4, OUTPUT);

  digitalWrite(N1, LOW);
  digitalWrite(N2, LOW);
  digitalWrite(N3, LOW);
  digitalWrite(N4, LOW);
}

void loop() {
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
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


      if (xAxis < 70) {
        left(); 
        //delay(10);

      } else if (xAxis > 260) {
        right();
        //delay(10);

      } else if (distance < 80) {
        forward();
        //delay(10);


      } else if (distance > 110) {
        backward();
        //delay(10);

      } else {
        stopp();
      }


    } else {
      Serial.println("vision body undetected.");
      stopp();
    }
  }


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

void forward () {
  analogWrite(speedPinA, speedA);
  digitalWrite(N1, LOW );
  digitalWrite(N2, HIGH);

  analogWrite(speedPinB, speedB);
  digitalWrite(N3, HIGH);
  digitalWrite(N4, LOW);
}

void backward () {
  analogWrite(speedPinA, speedA);
  digitalWrite(N1, HIGH );
  digitalWrite(N2, LOW );

  analogWrite(speedPinB, speedB);
  digitalWrite(N3, LOW);
  digitalWrite(N4, HIGH);
}

void left () {
  analogWrite(speedPinA, speedA);
  digitalWrite(N1, LOW);
  digitalWrite(N2, HIGH);
  
  analogWrite(speedPinB, speedB);
  digitalWrite(N3, LOW );
  digitalWrite(N4, HIGH );
}

void right () {
  analogWrite(speedPinA, speedA);
  digitalWrite(N1, HIGH);
  digitalWrite(N2, LOW);

  analogWrite(speedPinB, speedB);
  digitalWrite(N3, HIGH );
  digitalWrite(N4, LOW );
}

void stopp () {
  analogWrite(speedPinA, 0);
  digitalWrite(N1, LOW);
  digitalWrite(N2, LOW);
  
  analogWrite(speedPinB, 0);
  digitalWrite(N3, LOW);
  digitalWrite(N4, LOW);
}
