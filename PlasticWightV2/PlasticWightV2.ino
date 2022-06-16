
#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Pushbutton.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

const char* ssid     = "ESP8266-Весы";
const char* password = "";
String data = "0";

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 wight</h2>
  <p>
    <span class="dht-labels">wight</span> 
    <span id="wight">%WIGHT%</span>
    <sup class="units">&deg;grams</sup>
  </p>

</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("wight").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/wight", true);
  xhttp.send();
}, 10000 ) ;

</script>
</html>)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var) {
  Serial.println(var);
  if (var == "WIGHT") {
    return String(data);
  }
  return String();
}

void setupwifiAp() {
  // Serial port for debugging purposes
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/wight", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(data).c_str());
  });
  // Start server
  server.begin();
}


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = D6;
const int LOADCELL_SCK_PIN = D7;

HX711 scale;
int reading;
int lastReading;
//REPLACE WITH YOUR CALIBRATION FACTOR
#define CALIBRATION_FACTOR -122.507

//OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Button
#define BUTTON_PIN D5
Pushbutton button(BUTTON_PIN);

void displayWeight(int weight) {
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0, 10, "Weight plastic *grams"); // write something to the internal memory
  String weightstr = String(weight);
  u8g2.drawStr(20, 30, weightstr.c_str()); // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  setupwifiAp();

  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(CALIBRATION_FACTOR);   // this value is obtained by calibrating the scale with known weights
  scale.tare();               // reset the scale to 0
}

void loop() {

  if (button.getSingleDebouncedPress()) {
    Serial.print("tare...");
    scale.tare();
  }

  if (scale.wait_ready_timeout(200)) {
    reading = round(scale.get_units());
    Serial.print("Weight: ");
    Serial.println(reading);
    String dataScale = String(reading);
    data = dataScale;
    displayWeight(reading);
  }
  else {
    Serial.println("HX711 not found.");
  }
}
