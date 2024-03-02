#include "./oled_temp.h"
#include "./led.h"
#include "./status_led.h"

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

#define EnA 30
#define EnB 36

#define N1 31
#define N2 32
#define N3 34
#define N4 35

#define btn1 38

int pwmOutputright = 255;
int pwmOutputleft = 255;

int check = 0;

const int trig = 50;
const int echo = 51;
long duration, distance;

const int trig0 = 17;
const int echo0 = 16  ;
long duration0, distance0;

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX

void printResult(HUSKYLENSResult result);
int timer = 0;

#define rc3 9
#define rc4 6

int ch3;
int ch4;
int state0 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(btn1, INPUT);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo0, INPUT);
  pinMode(trig0, OUTPUT);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  temp11();
  display.display();
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  mySerial.begin(9600);
  while (!huskylens.begin(mySerial))
  {
    Serial.println(F("Begin failed!"));
    colortemp();
    display.setTextSize(1);
    display.clearDisplay();
    display.drawFastHLine(0, 10, 128, WHITE); //BLACK
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.print("  Please recheck the       connection");
    display.display();
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    Serial.println(F("2.Please recheck the connection."));
  }

  pinMode (rc3, INPUT);
  pinMode (rc4, INPUT);
  pinMode (EnA, OUTPUT);
  pinMode (EnB, OUTPUT);

  pinMode (N1, OUTPUT);
  pinMode (N2, OUTPUT);
  pinMode (N3, OUTPUT);
  pinMode (N4, OUTPUT);

  digitalWrite(N1, LOW);
  digitalWrite(N2, LOW);
  digitalWrite(N3, LOW);
  digitalWrite(N4, LOW);
  dht.setup(23);
  display.setTextSize(1);
  display.clearDisplay();
  display.drawFastHLine(0, 10, 128, WHITE); //BLACK
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("      connecting");
  display.display();
  delay(1000);
  temp11();

}
void loop() {
  while (check == 0) {
    if (digitalRead(btn1) == HIGH) {
      Fire2012(); // run simulation frame

      FastLED.show(); // display this frame
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      display.clearDisplay();
      display.setTextSize(1);



      display.setTextColor(WHITE);
      display.setCursor(0, 35);
      display.println(" Pls Switch to remote");
      display.display();
    } else {
      check = 1;
    }
  }


  while (check == 1) {
    timer += 10;
    Serial.println("check 1");
    Serial.println(timer);
    while (timer == 10000) {
      temp11();
      timer = 0;
    }
    led1();
    flysky();
    checkdis();
    checkdis0();
    if (distance <= 10) {
      backward();
      delay(500);
    }
    if (distance0 <= 5) {
      right();
      delay(100);
    }
    if (state0 == 0) {
      Remote_mode();
      delay(1000);
      temp11();
      state0 = 1;

    }

    if (digitalRead(btn1) == HIGH) {
      check = 2;
      state0 = 0;
    }
  }
  while (check == 2) {
    timer += 10;
    Serial.println("check 2");
    Serial.println(timer);
    while (timer == 10000) {
      temp11();
      timer = 0;
    }
    led2();
    control();
    checkdis();
    checkdis0();
    if (distance <= 20) {
      backward();
      delay(500);
    }
    if (distance0 <= 5) {
      right();
      delay(100);
    }

    if (state0 == 0) {
      AI_mode();
      delay(1000);
      temp11();
      state0 = 1;

    }
    if (digitalRead(btn1) == LOW) {
      check = 1;
      state0 = 0;
    }
  }
}

void checkdis() {
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print(distance);
  Serial.print(" cm\n");
}

void checkdis0() {
  digitalWrite(trig0, LOW);
  delayMicroseconds(5);
  digitalWrite(trig0, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig0, LOW);

  duration0 = pulseIn(echo0, HIGH);
  distance0 = (duration0 / 2) / 29.1;
  Serial.print(distance0);
  Serial.print(" cm\n");
}

void temp11() {
  delay(dht.getMinimumSamplingPeriod());

  float h = dht.getHumidity();
  float t = dht.getTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  display.clearDisplay();
  Serial.println(t);
  // display temperature
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(60, 0);
  display.println("Temperature ");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(60, 10);
  display.println(t);

  // display humidity
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(60, 35);
  display.println("Humidity ");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(60, 45);
  display.println(h);

  display.drawBitmap(0, 0, myBitmap, 128, 64, WHITE);
  display.display();

}
void control() {
  if (!huskylens.request()) {
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    colortemp();
  }
  if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if (!huskylens.available()) {
    Serial.println(F("No block or arrow appears on the screen!"));
    led2();
    analogWrite(EnA, 0);
    analogWrite(EnB, 0);
    stopmove();
  } else {
    Serial.println(F("###########"));
    if (huskylens.available())
    {
      ledlock();
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

      if (result.ID >= 1) {
        if (xAxis < 100) {
          analogWrite(EnA, 125);
          analogWrite(EnB, 125);
          left();
          //delay(10);

        } else if (xAxis > 230) {
          analogWrite(EnA, 125);
          analogWrite(EnB, 125);
          right();
          //delay(10);

        } else if (distance < 45  ) {
          analogWrite(EnA, pwmOutputright);
          analogWrite(EnB, pwmOutputleft);
          forward();
          //delay(10);


        } else if (distance > 60) {
          analogWrite(EnA, pwmOutputright);
          analogWrite(EnB, pwmOutputleft);
          backward();
          //delay(10);

        } else {
          analogWrite(EnA, 0);
          analogWrite(EnB, 0);
          stopmove();
        }


      } else {
        Serial.println("vision body undetected.");
        analogWrite(EnA, 0);
        analogWrite(EnB, 0);
        stopmove();
      }
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

void flysky() {
  Serial.println(ch3);
  Serial.println(ch4);
  ch3 = pulseIn(rc3, HIGH);
  ch4 = pulseIn(rc4, HIGH);
  analogWrite(EnA, pwmOutputright);
  analogWrite(EnB, pwmOutputleft);
  if (ch4 < 1500 && ch4 > 1300 && ch3 < 1500 && ch3 > 1300)
  {
    //stop
    stopmove();
  }

  if (ch4 > 1600)
  {
    if (ch3 < 1300)
    {

    }
    else
    {
      if (ch3 > 1600)
      {

      }
      else
      { //right
        right();
        /*digitalWrite(dirleft, HIGH); // Enables the motor to move in a particular direction
          digitalWrite(dirright, LOW);
          // Makes 200 pulses for making one full cycle rotation
          for (int x = 0; x < 1000; x++) {
          digitalWrite(stepleft, HIGH);
          delayMicroseconds(speeddo);
          digitalWrite(stepleft, LOW);
          delayMicroseconds(speeddo);
          digitalWrite(stepright, HIGH);
          delayMicroseconds(speeddo);
          digitalWrite(stepright, LOW);
          delayMicroseconds(speeddo);
          }*/
      }

    }
  }

  else
  {
    if (ch4 < 1300)
    {
      if (ch3 < 1300)
      {
        /*digitalWrite(EnA, HIGH);
          digitalWrite(N1, HIGH);
          digitalWrite(N2, LOW);

          digitalWrite(N3, LOW);
          digitalWrite(N4, LOW);
          digitalWrite(EnB, LOW);*/
      }
      else
      {
        if (ch3 > 1600)
        {
          /*digitalWrite(EnA, LOW);
            digitalWrite(N1, LOW);
            digitalWrite(N2, LOW);

            digitalWrite(N3, LOW );
            digitalWrite(N4, HIGH);
            digitalWrite(EnB, HIGH);*/
        }
        else
        { //left
          left();
          /*digitalWrite(dirleft, LOW); // Enables the motor to move in a particular direction
            digitalWrite(dirright, HIGH);
            // Makes 200 pulses for making one full cycle rotation
            for (int x = 0; x < 1000; x++) {
            digitalWrite(stepleft, HIGH);
            delayMicroseconds(speeddo);
            digitalWrite(stepleft, LOW);
            delayMicroseconds(speeddo);
            digitalWrite(stepright, HIGH);
            delayMicroseconds(speeddo);
            digitalWrite(stepright, LOW);
            delayMicroseconds(speeddo);
            }*/
        }
      }

    }
    else
    {
      if (ch3 < 1300)
      { //backward
        backward();
        /*digitalWrite(dirleft, HIGH); // Enables the motor to move in a particular direction
          digitalWrite(dirright, HIGH);
          // Makes 200 pulses for making one full cycle rotation
          for (int x = 0; x < 1000; x++) {
          digitalWrite(stepleft, HIGH);
          delayMicroseconds(speeddo);
          digitalWrite(stepleft, LOW);
          delayMicroseconds(speeddo);
          digitalWrite(stepright, HIGH);
          delayMicroseconds(speeddo);
          digitalWrite(stepright, LOW);
          delayMicroseconds(speeddo);
          }*/
      }
      else
      {
        if (ch3 > 1600)
        { //forward
          forward();
          /*digitalWrite(dirleft, LOW); // Enables the motor to move in a particular direction
            digitalWrite(dirright, LOW);
            // Makes 200 pulses for making one full cycle rotation
            for (int x = 0; x < 1000; x++) {
            digitalWrite(stepleft, HIGH);
            delayMicroseconds(speeddo);
            digitalWrite(stepleft, LOW);
            delayMicroseconds(speeddo);
            digitalWrite(stepright, HIGH);
            delayMicroseconds(speeddo);
            digitalWrite(stepright, LOW);
            delayMicroseconds(speeddo);
            }*/
        }
        else
        {
          stopmove();
          /*digitalWrite(EnA, LOW);
            digitalWrite(N1, LOW);
            digitalWrite(N2, LOW);

            digitalWrite(N3, LOW);
            digitalWrite(N4, LOW);
            digitalWrite(EnB, LOW);*/
        }
      }
    }
  }
}

void stopmove() {
  digitalWrite(EnA, LOW);
  digitalWrite(N1, LOW);
  digitalWrite(N2, LOW);

  digitalWrite(N3, LOW);
  digitalWrite(N4, LOW);
  digitalWrite(EnB, LOW); //stop
}
void forward() {
  digitalWrite(EnA, HIGH);
  digitalWrite(N1, HIGH);
  digitalWrite(N2, LOW);

  digitalWrite(N3, HIGH );
  digitalWrite(N4, LOW );
  digitalWrite(EnB, HIGH);
}
void backward() {
  digitalWrite(EnA, HIGH);
  digitalWrite(N1, LOW);
  digitalWrite(N2, HIGH);

  digitalWrite(N3, LOW );
  digitalWrite(N4, HIGH );
  digitalWrite(EnB, HIGH);
}
void left() {
  digitalWrite(EnA, HIGH);
  digitalWrite(N1, HIGH );
  digitalWrite(N2, LOW );

  digitalWrite(N3, LOW);
  digitalWrite(N4, HIGH);
  digitalWrite(EnB, HIGH);
}
void right() {
  digitalWrite(EnA, HIGH);
  digitalWrite(N2, HIGH);
  digitalWrite(N1, LOW );

  digitalWrite(N3, HIGH);
  digitalWrite(N4, LOW);
  digitalWrite(EnB, HIGH);
}

void AI_mode () {
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK );
  display.setCursor(15, 0);
  display.println("      MODE");
  display.drawFastHLine(0, 10, 128, WHITE); //BLACK
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("    AI     Tracking");
  display.display();
}
void Remote_mode() {
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK );
  display.setCursor(15, 0);
  display.println("      MODE");
  display.drawFastHLine(0, 10, 128, WHITE); //BLACK
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("  Remote");
  display.display();
}
