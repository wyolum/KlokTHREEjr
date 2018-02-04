#define PROGMEM

#include <FastLED.h>
#include <credentials.h>
#include <AdafruitIO_WiFi.h>
#include "config.h"

/* ---- credentials.h ----
char ssid[] = "XXX";  // your network SSID (name)
char pass[] = "YYY";  // your network password
*/

// huzzah
#define DATA_PIN     13
#define CLK_PIN      14

#define COLOR_ORDER BGR
#define LED_TYPE APA102
#define MILLI_AMPS 1000  // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#define NUM_LEDS (2 * 64)

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// AdafruitIO_Feed *time_seconds = io.feed("setTemp");
uint32_t current_time;
void printTime(uint32_t tm){
  uint8_t hh, mm, ss;
  char s[8];
  
  hh = (tm % 86400 / 3600) % 24;
  mm = (tm % 86400 / 60) % 60;
  ss = (tm % 86400) % 60;
  
  sprintf(s, "%02d:%02d:%02d", hh, mm, ss);
  Serial.println(s);
}

void handleTimeSeconds(uint32_t gmt){
  current_time = gmt;
  printTime(current_time);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  mqtt.connect();
  Serial.print("HERE!!!... ");
  
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    //Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }

  Serial.println("MQTT Connected!");
}

void setup(){
  Serial.begin(115200);
  delay(200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setDither(true);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(8);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();


  io.connect();
  int iii=0;
  Serial.print("Wait for IO connect.");
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    if(iii++ % 2){
      fill_solid(leds, NUM_LEDS, CRGB::Red);
    }
    else{
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
    }
    FastLED.show();
    delay(500);
  }
  Serial.println(io.statusText());Serial.println("Connected to IO");
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(1000);
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  time_seconds.setCallback(handleTimeSeconds);
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  Serial.println("mqtt.subscribe()");
  mqtt.subscribe(&time_seconds);
  Serial.println("mqtt subscribed");
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(1000);
  Serial.print("MQTT connect:");
  MQTT_connect();
  //Serial.println("mqtt connected");
}

void loop(){
}
