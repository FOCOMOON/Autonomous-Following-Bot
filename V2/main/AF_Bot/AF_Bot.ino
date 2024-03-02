#include "./FastLED.h"
#include "./husky.h"
#define rc1 20
#define rc2 21
#define rc3 19
#define buzzer 45

int ch1;
int ch2;
int ch3;
int speed_ch1F = 0;
int speed_ch1B = 0;
int speed_ch2L = 0;
int speed_ch2R = 0;

int speed_ch1F2 = 0;
int speed_ch1B2 = 0;
int speed_ch2L2 = 0;
int speed_ch2R2 = 0;

#define N1 5
#define N2 6
#define N3 7
#define N4 8

int state = 0;
int speed_left = 255;
int lowspeed_left = 5;
int speed_right = 255;
int lowspeed_right = 5;

int speed_now = 50;
int speed1;
int speed2;
int check_state = 2;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  setup_fastled();
  setup_husky();
  pinMode(buzzer, OUTPUT);
  pinMode (rc1, INPUT);
  pinMode (rc2, INPUT);
  pinMode (rc3, INPUT);
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);
  analogWrite(N1, 0);
  analogWrite(N2, 0);
  analogWrite(N3, 0);
  analogWrite(N4, 0);
  led2();
  delay(2000);
}

void loop() {
  //remote();
  //control();
  if (Serial.available()) {
    char receivedChar = Serial.read(); // Read the incoming byte
    Serial.write(receivedChar); // Echo the received byte back
    if (receivedChar == 'o'){
      speed_now = 70;
    }
    if (receivedChar == 'f'){
      speed_now = 150;
    }
    if (receivedChar == 'a') {
      web_mode();
      check_state = 1;
      Serial.println("\nweb mode");
    } else if (receivedChar == 'b') {
      check_state = 3;
      Serial.println("\nself AI mode"); 
    } else if (receivedChar == 'c') {
      check_state = 2;
      Serial.println("\nremote mode"); 
    }

    if (check_state == 1) {
      if (receivedChar == 'F') {
        forward(speed_now, speed_now);
      } else if (receivedChar == 'B') {
        backward(speed_now, speed_now);
      } else if (receivedChar == 'R') {
        right(speed_now, speed_now);
      } else if (receivedChar == 'L') {
        left(speed_now, speed_now);
      } else if (receivedChar == 'S') {
        stopmove();
      }
    } else if (check_state == 2) {
      remote();
    } else if (check_state == 3) {
      control();
    }

  }
}

void control() {
  if (!huskylens.request()) {
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    random16_add_entropy( random());

    Fire2012WithPalette(); // run simulation frame, using palette colors

    FastLED.show(); // display this frame
    FastLED.delay(10 / FRAMES_PER_SECOND);
  }
  if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if (!huskylens.available()) {
    Serial.println(F("No block or arrow appears on the screen!"));
    stopmove();
    led1();
  } else {
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

      if (result.ID >= 1) {
        ledlock();
        if (xAxis < 100) {
          left(50, 50);
          //delay(10);

        } else if (xAxis > 230) {
          right(50, 50);
          //delay(10);

        } else if (distance < 45  ) {
          forward(50, 50);
          //delay(10);


        } else if (distance > 60) {
          backward(50, 50);
          //delay(10);

        } else {
          stopmove();
        }


      } else {
        Serial.println("vision body undetected.");
        unknown();
        stopmove();
      }
    }
  }
}

void remote() {
  Serial.print("ch1 : ");
  Serial.println(ch1);
  Serial.print("ch2 : ");
  Serial.println(ch2);
  Serial.print("ch3 : ");
  Serial.println(ch3);
  ch1 = pulseIn(rc1, HIGH);
  ch2 = pulseIn(rc2, HIGH);
  ch3 = pulseIn(rc3, HIGH);
  speed_ch1F = map(ch1, 1600, 2000, lowspeed_right, speed_right);// config speed right wheel
  speed_ch1B = map(ch1, 1300, 995, lowspeed_right, speed_right);
  speed_ch2L = map(ch2, 1300, 995, lowspeed_right, speed_right);
  speed_ch2R = map(ch2, 1600, 2000, lowspeed_right, speed_right);

  speed_ch1F2 = map(ch1, 1600, 2000, lowspeed_left, speed_left);// config speed left wheel
  speed_ch1B2 = map(ch1, 1300, 995, lowspeed_left, speed_left);
  speed_ch2L2 = map(ch2, 1300, 995, lowspeed_left, speed_left);
  speed_ch2R2 = map(ch2, 1600, 2000, lowspeed_left, speed_left);

  if (!huskylens.request()) {
    stopmove();
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    random16_add_entropy( random());

    Fire2012WithPalette(); // run simulation frame, using palette colors

    FastLED.show(); // display this frame
    FastLED.delay(10 / FRAMES_PER_SECOND);
  } else {
    if (ch3 > 1600) {
      if (state == 0) {
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
        state = 1;
      }
      control();
    } else {
      if (state == 1) {
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
        state = 0;
      }
      colortemp();

      if (ch1 == 0 && ch2 == 0) {
        stopmove();
      } else {

        if (ch2 < 1500 && ch2 > 1300 && ch1 < 1500 && ch1 > 1300)
        {
          //stop
          stopmove();
          //Serial.println("stop move");
        }

        if (ch2 > 1600)
        { //right
          right(speed_ch2R, speed_ch2R);

        } else {

          if (ch2 < 1300)
          { //left
            left(speed_ch2L2, speed_ch2L);

          } else {

            if (ch1 < 1300)
            { //backward
              backward(speed_ch1B2, speed_ch1B);
              
            }
            else
            {
              if (ch1 > 1600)
              { //forward
                forward(speed_ch1F2, speed_ch1F);

              }
              else
              {
                stopmove();
              }
            }
          }
        }
      }
    }
  }
}

void forward(int speed1, int speed2) {
  digitalWrite(N1, LOW);
  analogWrite(N2, speed2);

  digitalWrite(N3, LOW);
  analogWrite(N4, speed1);
}

void backward(int speed1, int speed2) {
  digitalWrite(N2, LOW);
  analogWrite(N1, speed2);

  digitalWrite(N4, LOW);
  analogWrite(N3, speed1);
}

void stopmove() {
  digitalWrite(N2, LOW);
  digitalWrite(N1, LOW);

  digitalWrite(N4, LOW);
  digitalWrite(N3, LOW);
}

void left(int speed1, int speed2) {
  digitalWrite(N2, LOW);
  analogWrite(N1, speed2);

  digitalWrite(N3, LOW);
  analogWrite(N4, speed1);
}

void right(int speed1, int speed2) {
  digitalWrite(N1, LOW);
  analogWrite(N2, speed2);

  digitalWrite(N4, LOW);
  analogWrite(N3, speed1);
}
