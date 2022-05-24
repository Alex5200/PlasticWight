#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS         D0
#define TFT_RST        D3                                           
#define TFT_DC         D4

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

class SetupTFTDisplayconnect {
  public:
    void setupDisplay() {
      tft.initR(INITR_BLACKTAB);
      uint16_t time = millis();
      tft.fillScreen(ST77XX_BLACK);
      time = millis() - time;
      tft.fillScreen(ST77XX_BLACK);
    }
    void draw_text(char *text) {
      tft.setCursor(0, 0);
      tft.setTextWrap(true);
      tft.print(text);
    }
};
