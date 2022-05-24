#include "HX711.h"
HX711 scale;

uint8_t pin_data  = D2;//for esp32
uint8_t pin_clock = D1;//for esp32
uint8_t pin_button = D6;

#include "./tftDisplaySetup.h"

SetupTFTDisplayconnect tftDisp;

void setupHX711() {
  pinMode(pin_button, INPUT);
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  scale.begin(pin_data, pin_clock);
  
  tft.fillScreen(ST77XX_BLACK);
  tftDisp.draw_text("UNITS: ");
  tft.print(String(scale.get_units(10)));
  Serial.println(scale.get_units(10));
  Serial.println(digitalRead(pin_button));

  tft.fillScreen(ST77XX_BLACK);
  tftDisp.draw_text("\nEmpty the scale, press a key to continue");
  Serial.println(digitalRead(pin_button));
  while (digitalRead(pin_button) == 0);
  delay(3000);

  scale.tare();
  tft.fillScreen(ST77XX_BLACK);
  tftDisp.draw_text("UNITS: ");
  tft.print(String(scale.get_units(10)));
  Serial.println(scale.get_units(10));
  
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  tftDisp.draw_text("\nPut 1000 gram in the scale, press a key to continue");
  while (digitalRead(pin_button) == 0);
  delay(3000);


  scale.calibrate_scale(0, 5);
  tft.fillScreen(ST77XX_BLACK);
  tftDisp.draw_text("UNITS: ");
  tft.print(scale.get_units(10));
    Serial.println(scale.get_units(10));

}

int get_intData() {
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));
  delay(250);
  return scale.get_units(10);
}


void setup() {
  Serial.begin(115200);
  tftDisp.setupDisplay();
  setupHX711();
}

void loop() {
  tftDisp.draw_text("UNITS: ");
  tft.print(scale.get_units(10));
  delay(2000);
}
