#include "HX711.h"
HX711 scale;

uint8_t dataPin  = 6;//for esp32
uint8_t clockPin = 7;//for esp32

class WorkHX711 {
  public:
    void setupHX711(int pin_data, int pin_clock, int pin_button) {
      pinMode(pin_button, INPUT);
      Serial.begin(115200);
      Serial.println(__FILE__);
      Serial.print("LIBRARY VERSION: ");
      Serial.println(HX711_LIB_VERSION);
      Serial.println();

      scale.begin(pin_data, pin_clock);
      
//      tfts.draw_text("UNITS: ");
//      tft.print(String(scale.get_units(10)));

//      tfts.draw_text("\nEmpty the scale, press a key to continue");
      while (!digitalRead(pin_button));
      while (digitalRead(pin_button)) Serial.read();

      scale.tare();
//      tfts.draw_text("UNITS: ");
//      tft.print(String(scale.get_units(10)));
//
//      tfts.draw_text("\nPut 1000 gram in the scale, press a key to continue");
//      while (!digitalRead(pin_button));
      while (digitalRead(pin_button)) Serial.read();

      scale.calibrate_scale(0, 5);
//      tfts.draw_text("UNITS: ");
//      tft.print(scale.get_units(10));
    }

    int get_intData() {
      Serial.print("UNITS: ");
      Serial.println(scale.get_units(10));
      delay(250);
      return scale.get_units(10);
    }
};
