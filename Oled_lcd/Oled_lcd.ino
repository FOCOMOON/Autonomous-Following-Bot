#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

const int btn = 7;
int modeCounter = 0;
int led_status = 0;

void setup() {
  Serial.begin(9600);
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
  if(digitalRead(btn)==1){
  if(led_status==0){
    Remote_mode();
    Serial.println("mode0");
    led_status = 1;
  }else if(led_status==1){
    gps_mode();
    Serial.println("mode1");
    led_status = 2;
  }else if (led_status = 2){
    AI_mode();
    Serial.println("mode2");
    led_status = 0;
  }
  delay(250);
  }
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
