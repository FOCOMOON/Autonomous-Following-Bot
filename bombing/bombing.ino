#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

const int btn = 7;
int modeCounter = 0;
int led_status = 0;
int check = 0;

#define rc3 9
#define rc4 6

int ch3;
int ch4;

#define EnA 30
#define EnB 36

#define N1 31
#define N2 32
#define N3 34
#define N4 35

int pwmOutputright = 255;
int pwmOutputleft = 255;

void setup() {
  Serial.begin(9600);
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

  pinMode(btn, INPUT_PULLUP);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK );
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("  Click Me   Plz ><");
  display.display();

}

void loop() {
  analogWrite(EnA, pwmOutputright);
  analogWrite(EnB, pwmOutputleft);
  if (digitalRead(btn) == 1) {
    if (led_status == 0) {
      check = 1;
      led_status = 1;
    } else if (led_status == 1) {
      check = 2;
      led_status = 2;
    } else if (led_status = 2) {
      check = 3;
      led_status = 0;
    }
    delay(250);
  }

  if (check == 1) {
    Remote_mode();
    flysky();
    Serial.println("remote");
  } else if (check == 2) {
    gps_mode();
    Serial.println("gps");
  } else if (check == 3) {
    Serial.println("ai");
    AI_mode();
  }

}
void flysky() {
  Serial.println(ch3);
  Serial.println(ch4);
  ch3 = pulseIn(rc3, HIGH);
  ch4 = pulseIn(rc4, HIGH);
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


void gps_mode() {
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK );
  display.setCursor(15, 0);
  display.println("      MODE");
  display.drawFastHLine(0, 10, 128, WHITE); //BLACK
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("    GPS    Tracking");
  display.display();
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
