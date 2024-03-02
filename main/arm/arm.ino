const int stepPin = 3; //X.STEP
const int dirPin = 2; // X.DIR
const int stepPin1 = 5; //X.STEP
const int dirPin1 = 4; // X.DIR
const int stepPin2 = 10; //X.STEP
const int dirPin2 = 11; // X.DIR

#define rc3 9
#define rc4 6
#define rc5 8

int ch3;
int ch4;
int ch5;
int SMSpeed = 200;

void setup() {
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode (rc3, INPUT);
  pinMode (rc4, INPUT);
  pinMode (rc5, INPUT);

}

void loop() {
  ch3 = pulseIn(rc3, HIGH);
  ch4 = pulseIn(rc4, HIGH);
  ch5 = pulseIn(rc5, HIGH);
  /*Serial.print("ch3 ");
  Serial.println(ch3);
  Serial.print("ch4 ");
  Serial.println(ch4);
  Serial.print("ch5 ");
  Serial.println(ch5);*/
  if (ch5 < 1290) {
    digitalWrite(dirPin2, LOW);

    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(SMSpeed);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(SMSpeed);
  } else if (ch5 > 1600) {
    digitalWrite(dirPin2, HIGH);

    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(SMSpeed);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(SMSpeed);
  }
  if (ch4 < 1500 && ch4 > 1300 && ch3 < 1500 && ch3 > 1300)
  {
    //stop
    /*stopmove();*/
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
        digitalWrite(dirPin, LOW);

        digitalWrite(stepPin, HIGH);
        delayMicroseconds(SMSpeed);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(SMSpeed);
        /*right();
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
          digitalWrite(dirPin, HIGH);

          digitalWrite(stepPin, HIGH);
          delayMicroseconds(SMSpeed);
          digitalWrite(stepPin, LOW);
          delayMicroseconds(SMSpeed);
          /*left();
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
        digitalWrite(dirPin1, LOW);

        digitalWrite(stepPin1, HIGH);
        delayMicroseconds(SMSpeed);
        digitalWrite(stepPin1, LOW);
        delayMicroseconds(SMSpeed);
        /*backward();
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
          digitalWrite(dirPin1, HIGH);

          digitalWrite(stepPin1, HIGH);
          delayMicroseconds(SMSpeed);
          digitalWrite(stepPin1, LOW);
          delayMicroseconds(SMSpeed);
          /*forward();
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
          /*stopmove();
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
