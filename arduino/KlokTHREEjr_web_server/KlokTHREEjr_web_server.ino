#include <FastLED.h>
#include <WiFiManager.h>
#include <EEPROMAnything.h>
#define ULTIM8x16
#include <MatrixMaps.h>

#include "textures.h"
#include "logic.h"
#include "Faceplate.h"
#include "clock.h"

#include "dutch_v1.h"
#include "french_v1.h"
#include "german_v3.h"
#include "hebrew_v1.h"
#include "hungarian_v2.h"
#include "irish_v1.h"
#include "italian_v1.h"
#include "english_v3.h"
#include "spanish_v1.h"

#include "config.h"


const bool ON = true;
const bool OFF = !ON;

// How many leds are in the strip?
const uint8_t N_BOARD = 2;
const uint8_t NUM_LEDS = 64 * N_BOARD;
bool mask[NUM_LEDS];
bool wipe[NUM_LEDS];
CRGB leds[NUM_LEDS];

#define DATA_PIN     13
#define CLK_PIN      14
#define COLOR_ORDER BGR
#define LED_TYPE APA102
#define MILLI_AMPS 1000  // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)

uint8_t logo_rgb[] = {
  0x11,0x00,0x29,0x00,0x25,0x00,0x23,0x00,0x25,0x00,0x29,0x00,0x31,0x00,0xe0,0x01,
  0x00,0x03,0x80,0x04,0x80,0x04,0x80,0x04,0x80,0x04,0x00,0x03,0x00,0x00,0x00,0x00,
  0x11,0x00,0x09,0x88,0x05,0x48,0x03,0x28,0x05,0x18,0x09,0x28,0x11,0x48,0x00,0x88
};

Faceplate faceplates[] = {
  english_v3,
  spanish_v1
};
uint8_t num_faceplates = 2;

struct config_t{
  int timezone;
  long alarm;
  int mode;
  uint8_t brightness;
  uint8_t display_idx;
} configuration;

void display_bitmap_rgb(uint8_t* bitmap){
  uint8_t n = 16;
  uint8_t h = 8;
  uint8_t w = 16;
  uint8_t xy[2];
  uint8_t r, g, b;
  
  int i, j;
  int led_idx, byte_idx;

  //struct CRGB color;
  int x, y;
  
  for(i=0; i<n; i++){
    r = bitmap[i + 0 * w];
    g = bitmap[i + 1 * w];
    b = bitmap[i + 2 * w];
    //   012345678pabcdef
    // 0 0000000011111111
    // 1 2222222233333333
    // 2 4444444455555555
    // 3 6666666677777777
    // 4 8888888899999999
    // 5 aaaaaaaabbbbbbbb
    // 6 ccccccccdddddddd
    // 7 eeeeeeeeffffffff
    for(j=0; j<8; j++){
      x = (i * 8 + j) % 16;
      y = i / 2;
      led_idx = XY(x, y);
      leds[led_idx].red   = 255 * ((r >> j) & 1);
      leds[led_idx].green = 255 * ((g >> j) & 1);
      leds[led_idx].blue  = 255 * ((b >> j) & 1);
      if(leds[led_idx].red || leds[led_idx].green || leds[led_idx].blue){
	mask[led_idx] = true;
      }
    }
  }
}

void apply_mask(bool* mask){
  uint16_t b, k;
  for(uint16_t i=0; i < NUM_LEDS; i++){
    if(!mask[i]){
      leds[i] = CRGB::Black;
    }
  }
}

void wipe_around(bool val){
  float dtheta = 31.4 / 180;
  float theta = -3.14 - dtheta;
  int row, col;
  bool tmp[NUM_LEDS];

  int cx = random(0, MatrixWidth-1);
  int cy = random(0, MatrixHeight-1);
  cx = 8;
  cy = 4;
  
  fillMask(wipe, !val);
  while (theta < 3.14 + dtheta){
    for(row=0; row < MatrixHeight; row++){
      for(col=0; col < MatrixWidth; col++){
	if(atan2(row - cy, col - cx) < theta){
	  setPixelMask(wipe, row, col, val);
	}
      }
    }
    logical_or(NUM_LEDS, wipe, mask, tmp);    
    rainbow(leds, 0, XY);
    apply_mask(tmp);
    FastLED.show();
    theta += dtheta;
  }
}

void fillMask(bool* mask, bool b){
  fillMask(mask, b, 0, NUM_LEDS);
}

void fillMask(bool* mask, bool b, int start, int stop){
  for(int i = start; i < stop && i < NUM_LEDS; i++){
    mask[i] = b;
  }
}

void setPixelMask(bool* mask, uint8_t row, uint8_t col, bool b){
  if(row >= MatrixHeight){
  }
  else if(col >= MatrixWidth){
  }
  else{
    uint16_t pos = XY(col, row);
    if(pos < NUM_LEDS){
      mask[pos] = b;
    }
  }
}

void loadSettings(){
  EEPROM_readAnything(0, configuration);  
}

void saveSettings(){
  EEPROM_writeAnything(0, configuration);
  EEPROM.commit();
}

uint16_t XY( uint8_t x, uint8_t y){
  uint16_t out = 0;
  if(FLIP_DISPLAY){
    x = MatrixWidth - x - 1;
    y = MatrixHeight - y - 1;
  }
  if(x < MatrixWidth && y < MatrixHeight){
    out = MatrixMap[y][x];
  }
  return out;
}

void led_setup(){
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setDither(true);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  FastLED.setBrightness(10);
  //FastLED.setBrightness(configuration.brightness);
  wipe_around(ON);
  display_bitmap_rgb(logo_rgb);
  wipe_around(OFF);
  display_bitmap_rgb(logo_rgb);
  FastLED.show();
}

NTPClock ntp_clock;

void setup(){
  Serial.begin(115200);
  delay(200);
  Serial.println("setup() starting");
  loadSettings();
  led_setup();

  for(int ii = 0; ii < num_faceplates; ii++){
    faceplates[ii].setup(MatrixWidth, MatrixHeight, XY);
  }

  WiFiManager wifiManager;
  wifiManager.autoConnect("KLOK");
  Serial.println("yay connected");
  
  Serial.println("setup() complete");
}

uint32_t count;

void loop(){
  uint8_t word[3];
  //uint32_t current_time;

  uint32_t current_time = ntp_clock.now();
  fillMask(mask, OFF);
  faceplates[0].maskTime(current_time, mask);
  rainbow(leds, count, XY);
  apply_mask(mask);
  FastLED.show();
  Serial.println(current_time);
  delay(1000);
}
