#include <AdafruitIO_WiFi.h>
#include <EEPROM.h>
#include <EEPROMAnything.h>
#include "config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <WiFiClientSecure.h>
#include <credentials.h> //see arduino/libraries/credentials/README for a credentials.h template
#define ULTIM8x16
#include <MatrixMaps.h>

#define WIFI_MANAGER

WiFiClientSecure *client_p;

//#include "dutch_v1.h"
// #include "english_v0.h"
// #include "english_v2.h"
#include "english_v3.h"
// #include "english_v4.h"
// #include "french_v1.h"
// #include "german_v1.h"
// #include "german_v2.h"
// #include "german_v3.h"
// #include "german_v4.h"
// #include "german_v5.h"
// #include "hebrew_v1.h"
// #include "hungarian_v1.h"
// #include "hungarian_v2.h"
// #include "irish_v1.h"
// #include "spanish_v1.h"

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "us.pool.ntp.org", 0, 60000);

struct config_t{
  int timezone;
  long alarm;
  int mode;
  uint8_t brightness;
  uint8_t display_idx;
} configuration;

uint32_t current_time;

typedef void (*Init)();
typedef void (*Background)();
typedef void (*Transition)(uint16_t last_time_inc, uint16_t time_inc);
void minutes_hack(uint8_t i, bool *out);
void get_time_display(bool* mask, int i);
void fill_red();
void fill_green();
void fill_blue();

typedef struct{
  Init       init;
  Background background;
  Transition transition;
  String     name;
  int        id;
} Display;

typedef Display Displays[];

void noop(){
}
void blend_to_rainbow();

Display WordDropDisplay = {blend_to_rainbow, rainbow, word_drop, String("Word Drop"), 0};
Display WipeAroundDisplay = {blend_to_rainbow, rainbow, wipe_around_transition, String("Wipe Around"), 1};
Display TheMatrixDisplay = {blend_to_blue, fill_blue, TheMatrix, String("The Matrix"), 2};

const int N_DISPLAY = 3;
Display *Display_ps[N_DISPLAY] = {&WordDropDisplay,
				  &WipeAroundDisplay,
				  &TheMatrixDisplay};
Display* CurrentDisplay_p = &WipeAroundDisplay;

void new_display(Display* Display_p){
  Display_p->init();
  CurrentDisplay_p = Display_p;
}

// language constants
uint8_t n_minute_led;               // number of minute hack leds
uint8_t n_minute_state;             // number of minute hack states to cycle through
uint8_t n_byte_per_display = 4;         // number of bytes used for each 5 minunte time incriment

uint8_t display_idx;
uint8_t last_min_hack_inc = 0;
uint16_t last_time_inc = 0;

/* The display memory buffer is larger than physical display for screen 
 * staging and fast display.
 */
const uint8_t MAX_BRIGHTNESS = 50;
const bool ON = true;
const bool OFF = false;

// How many leds are in the strip?
#define N_BOARD 2
#define NUM_LEDS 64 * N_BOARD
bool mask[NUM_LEDS];
bool wipe[NUM_LEDS];

//// LOLIN32
// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLK_PIN 12

#define DATA_PIN     13
#define CLK_PIN      14

#define COLOR_ORDER BGR
#define LED_TYPE APA102
#define MILLI_AMPS 1000  // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
CRGB buffer[2 * NUM_LEDS];


void blend_to_rainbow(){
  int i;
  CHSV newcolor;
  int count = ((current_time % 300) * 255) / 300;
  
  newcolor.val = 255;
  newcolor.sat = 255;
  for(int ii=0; ii<NUM_LEDS; ii++){
    for( int row = 0; row < MatrixHeight; row++) {
      for( int col = 0; col < MatrixWidth; col++) {
	i = XY(col, row);
	if(mask[i]){
	  newcolor.hue =  (count + (MatrixWidth * row + col) * 2) % 256;
	  nblend(leds[XY(col, row)], newcolor, 1);
	}
      }
    }
    FastLED.show();
    delay(1);
  }
}

void blend_to_color(CRGB color){
  for(int kk=0; kk<128; kk++){
    for(int ii=0; ii<NUM_LEDS; ii++){
      if(mask[ii]){
	nblend(leds[ii], color, 1);
      }
    }
    FastLED.show();
    delay(1);
  }
}

void blend_to_red(){
  blend_to_color(CRGB::Red);
}

void blend_to_green(){
  blend_to_color(CRGB::Green);
}

void blend_to_blue(){
  blend_to_color(CRGB::Blue);
}

void fill_red(){
  fill_solid(leds, NUM_LEDS, CRGB::Red);
}
void fill_green(){
  fill_solid(leds, NUM_LEDS, CRGB::Green);
}
void fill_blue(){
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
}

void printTime(uint32_t tm){
  uint8_t hh, mm, ss;
  char s[8];
  
  hh = (tm % 86400 / 3600) % 24;
  mm = (tm % 86400 / 60) % 60;
  ss = (tm % 86400) % 60;
  
  sprintf(s, "%02d:%02d:%02d", hh, mm, ss);
  Serial.println(s);
}

void handleTimeSeconds(uint32_t tm){
  current_time = tm;
  printTime(current_time);
}

void setup(){
  Serial.begin(115200);
  delay(200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setDither(true);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();


#ifndef CONNECT_TO_ADA_IO  
  io.connect();
  Serial.print("Wait for IO connect.");
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(io.statusText());Serial.println("Connected to IO");
#endif
  timezone->onMessage(handleTimezone);
  brightness->onMessage(handleBrightness);
  display_idx_io->onMessage(handleDisplayIdx);
  EEPROM.begin(512);
  loadSettings();
  new_display(Display_ps[configuration.display_idx % N_DISPLAY]);
  
  FastLED.setBrightness(configuration.brightness);
  
  timeClient.setTimeOffset(configuration.timezone * 60);
  timeClient.begin();
  CurrentDisplay_p->init();
}

void loadSettings(){
  EEPROM_readAnything(0, configuration);  
}

void saveSettings(){
  EEPROM_writeAnything(0, configuration);
  EEPROM.commit();
}

void handleTimezone(AdafruitIO_Data *message){
  char *data = (char*)message->value();
  int dataLen = strlen(data);
  if (dataLen >= 0) {
    String dataStr = String(data);
    int new_timezone = (int)dataStr.toInt();
    if (new_timezone != configuration.timezone && -1440/2 < new_timezone && new_timezone <= 1440/2){
      configuration.timezone = new_timezone;
      timeClient.setTimeOffset(configuration.timezone * 60);
      saveSettings();
    }
  }  

  // print out the received value
  Serial.print(message->feedName());
  Serial.print(" ");
  Serial.println(message->value());
}

void handleBrightness(AdafruitIO_Data *message){
  char *data = (char*)message->value();

  int dataLen = strlen(data);
  if (dataLen > 0) {
    String dataStr = String(data);
    int new_brightness = (int)dataStr.toInt();
    if ((new_brightness != configuration.brightness) && (0 <= new_brightness) && (new_brightness <= MAX_BRIGHTNESS)){
      configuration.brightness = new_brightness;
      FastLED.setBrightness(configuration.brightness);
      saveSettings();
    }
  }
  // print out the received value
  Serial.print(message->feedName());
  Serial.print(" ");
  Serial.println(message->value());
}

void handleDisplayIdx(AdafruitIO_Data *message){
  char *data = (char*)message->value();

  int dataLen = strlen(data);
  if (dataLen > 0) {
    String dataStr = String(data);
    int new_idx = (int)dataStr.toInt() % N_DISPLAY;
    if ((new_idx != configuration.display_idx) && (0 <= new_idx) && (new_idx <= MAX_BRIGHTNESS)){
      configuration.display_idx = new_idx;
      new_display(Display_ps[new_idx]);
      saveSettings();
    }
  }
  // print out the received value
  Serial.print(message->feedName());
  Serial.print(" ");
  Serial.println(message->value());
}

const struct CRGB color = CRGB::White;

uint16_t XY( uint8_t x, uint8_t y)
{
  if(FLIP_DISPLAY){
    x = MatrixWidth - x - 1;
    y = MatrixHeight - y - 1;
  }
  uint16_t out = 0;
  if(x < MatrixWidth && y < MatrixHeight){
    out = MatrixMap[y][x];
  }
  return out;
}

typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

// Pride2015 by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend( leds[pixelnumber], newcolor, 64);
  }
}

void pride2()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint8_t x = 0; x < MatrixWidth; x++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    for (uint8_t y = 0; y < MatrixHeight; y++) {
      nblend(leds[XY(x, y)], newcolor, 64);
    }
  }
}

PatternAndNameList patterns = {
  { pride,                  "Pride" },
  { pride2,                 "Pride 2" }
};

void rainbow() {
  int i, dx, dy;
  CHSV hsv;
  float dist;
  
  hsv.hue = 0;
  hsv.val = 255;
  hsv.sat = 240;
  int count = ((current_time % 300) * 255) / 300;
  for( int row = 0; row < MatrixHeight; row++) {
    for( int col = 0; col < MatrixWidth; col++) {
      dy = (row - 4) * 2;
      dx = col - 8;
      dist = sqrt(dx * dx + dy * dy);
      i = XY(col, row);
      //hsv.hue =  ((int)(dist * 16) - count) % 256;
      hsv.hue =  (count + (MatrixWidth * row + col) * 2) % 256;
      leds[i] = hsv;
    }
  }
  // Show the leds (only one of which is set to white, from above)
  //delay(100);
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

void setWordMask(bool *mask, uint8_t* word, bool b){
  // word = [row, col, len]
  for(int i=0; i < word[2]; i++){
    setPixelMask(mask, word[1], word[0] + i, b);
  }
}

void logical_and(int n, bool* mask, bool* wipe, bool* out){
  int i;
  for(i = 0; i < n; i++){
    out[i] = mask[i] && wipe[i];
  }
}

void logical_or(int n, bool* mask, bool* wipe, bool* out){
  int i;
  for(i = 0; i < n; i++){
    out[i] = mask[i] || wipe[i];
  }
}

void logical_copy(int n, bool* mask, bool* out){
  int i;
  for(i = 0; i < n; i++){
    out[i] = mask[i];
  }
}
  
void logical_not(int n, bool* mask, bool* out){
  int i;
  for(i = 0; i < n; i++){
    out[i] = !mask[i];
  }
}

bool logical_equal(int n, bool* mask, bool* wipe){
  bool out;
  int i = 0;
  while(i < n && mask[i] == wipe[i]){
    i++;
  }
  return i == n;
}
bool all_true(int n, bool* mask){
  int i = 0;
  
  while(i < n && mask[i]){
    i++;
  }
  return i == n;
}

bool any_false(int n, bool* mask){
  return !all_true(n, mask);
}

bool all_false(int n, bool* mask){
  int i = 0;
  
  while(i < n && !mask[i]){
    i++;
  }
  return i == n;
}

bool any_true(int n, bool* mask){
  return !all_false(n, mask);
}


void apply_mask(bool* mask){
  uint16_t b, k;
  for(uint16_t i=0; i < NUM_LEDS; i++){
    if(!mask[i]){
      leds[i] = CRGB::Black;
    }
  }
}

const int32_t UPDATE_INTERVAL_MS = 1000;
int32_t last_update_ms= 0;
const int32_t UPDATE_IO_MS = 5000;
int32_t last_io_update_ms= 0;

void loop(){
  desk_clock();
}

void word_drop_in(uint16_t time_inc){
  uint8_t bits;     // holds the on off state for 8 words at a time
  uint8_t word[3];  // start columm, start row, length of the current word
  bool tmp_mask[NUM_LEDS];
  uint8_t tmp_word[3];
  
  fillMask(mask, false);
  fillMask(wipe, false);
  fillMask(tmp_mask, false);
  
  for(uint8_t j = 0; j < n_byte_per_display; j++){ // j is a byte index 
    // read the state for the next set of 8 words
    bits = pgm_read_byte(DISPLAYS + 1 + (time_inc * n_byte_per_display) + j);
    for(uint8_t k = 0; k < 8; k++){                     // k is a bit index
      if((bits >> k) & 1){                              // check to see if word is on or off
	getword(j * 8 + k, word);                       // if on, read location and length
	tmp_word[0] = word[0];
	tmp_word[1] = word[1];
	tmp_word[2] = word[2];
	for(int rr = 0; rr <= word[1]; rr++){
	  tmp_word[1] = rr;
	  setWordMask(wipe, tmp_word, true);
	  logical_or(NUM_LEDS, mask, wipe, tmp_mask);
	  rainbow();
	  apply_mask(tmp_mask);
	  FastLED.show();
	  delay(25);
	}
	setWordMask(mask, word, true);
	for(int rr = 0; rr < word[1]; rr++){
	  tmp_word[1] = rr;
	  setWordMask(wipe, tmp_word, false);
	  logical_or(NUM_LEDS, mask, wipe, tmp_mask);
	  
	  rainbow();
	  apply_mask(tmp_mask);
	  FastLED.show();
	  delay(25);
	}
      }
    }
  }
}
void word_drop_out(uint16_t time_inc){
  uint8_t bits;     // holds the on off state for 8 words at a time
  uint8_t word[3];  // start columm, start row, length of the current word
  bool tmp_mask[NUM_LEDS];
  uint8_t tmp_word[3];
  
  //fillMask(mask, false);
  //fillMask(wipe, false);
  //fillMask(tmp_mask, false);
  logical_copy(NUM_LEDS, mask, wipe);
  logical_copy(NUM_LEDS, mask, tmp_mask);
  
  for(uint8_t j = 0; j < n_byte_per_display; j++){ // j is a byte index 
    // read the state for the next set of 8 words
    bits = pgm_read_byte(DISPLAYS + 1 + (time_inc * n_byte_per_display) + j);
    for(uint8_t k = 0; k < 8; k++){                     // k is a bit index
      if((bits >> k) & 1){                              // check to see if word is on or off
	getword(j * 8 + k, word);                       // if on, read location and length
	tmp_word[0] = word[0];
	tmp_word[1] = word[1];
	tmp_word[2] = word[2];
	for(int rr = word[1]; rr <= 8; rr++){
	  tmp_word[1] = rr;
	  setWordMask(wipe, tmp_word, true);
	  logical_or(NUM_LEDS, mask, wipe, tmp_mask);
	  rainbow();
	  apply_mask(tmp_mask);
	  FastLED.show();
	  delay(25);
	}
	setWordMask(mask, word, false);
	for(int rr = word[1]; rr <= 8; rr++){
	  tmp_word[1] = rr;
	  setWordMask(wipe, tmp_word, false);
	  logical_or(NUM_LEDS, mask, wipe, tmp_mask);
	  
	  rainbow();
	  apply_mask(tmp_mask);
	  FastLED.show();
	  delay(25);
	}
      }
    }
  }
}

void word_drop(uint16_t last_time_inc, uint16_t time_inc){
  bool tmp_d[NUM_LEDS];

  rainbow();

  // swipe rainbow from the left
  //wipe_around(ON);
  //delay(1000);
  if(last_time_inc != 289){
    word_drop_out(last_time_inc);
  }
  
  // clear the new display
  fillMask(tmp_d, false);
  
  // read display for next time incement
  get_time_display(mask, time_inc);
  
  // clear rainbow to reveal the time
  //wipe_off_left();
  //wipe_around(OFF);
  word_drop_in(time_inc);
}

void fade_transition(uint16_t time_inc, uint16_t last_time_inc){
  int i, j;
  
  fillMask(mask, false);
  get_time_display(mask, last_time_inc);
  rainbow();
  apply_mask(mask); // fill leds with next time inc and copy to second half of buffer
  for(j = 0; j < NUM_LEDS; j++){
    buffer[j] = leds[j]; // buffer has new time
  }

  fillMask(mask, false);
  get_time_display(mask, time_inc); // leds have old time
  rainbow();
  apply_mask(mask);

  int rate = 1;
  for(j = 0; j < 255; j++){
    for(i=0; i < NUM_LEDS; i++){
      leds[i].red   = blend8(leds[i].red,   buffer[i].red, rate);
      leds[i].green = blend8(leds[i].green, buffer[i].green, rate);
      leds[i].blue  = blend8(leds[i].blue,  buffer[i].blue, rate);
    }
    FastLED.show();
    delay(1);
  }
}

void TheMatrix(uint16_t last_time_inc, uint16_t time_inc){
  int n_drop = 0;
  int n_need = 8;
  
  const struct CRGB color = CRGB::Green;
  uint8_t cols[NUM_LEDS];
  uint8_t rows[NUM_LEDS];
  uint8_t pause[NUM_LEDS];
  bool have[NUM_LEDS];
  int col;
  int i, j;

  // rainbow();
  fillMask(mask, false);
  fillMask(wipe, false);
  fillMask(have, false);
  get_time_display(mask, last_time_inc);
  get_time_display(wipe, time_inc);
  apply_mask(mask);
  FastLED.show();
  for(i=0; i < MatrixWidth; i++){
    for(j=0; j < MatrixHeight; j++){
      if(leds[XY(i, j)].red > 0 ||
	 leds[XY(i, j)].green > 0 ||
	 leds[XY(i, j)].blue > 0){
	rows[n_drop] = j;
	cols[n_drop] = i;
	n_drop++;
      }
      if(wipe[XY(i, j)]){
	n_need++;
      }
    }
  }
  
  delay(10);
  for(j = 0; j < 255 * 3; j++){
    for(i=0; i < NUM_LEDS; i++){
      leds[i].red   = blend8(leds[i].red, 0, 1);
      leds[i].green = blend8(leds[i].green, 255, 1);
      leds[i].blue  = blend8(leds[i].blue, 0, 1);
    }
    apply_mask(mask);
    FastLED.show();
    delay(5);
  }

  for(i = n_drop; i < n_need; i++){/// add enough drops to complete
    cols[i] = random(0, MatrixWidth);
    rows[i] = -random(0, MatrixHeight);
    n_drop++;
  }

  int end = millis() + 5000; // go for 5 seconds
  // while new display is not filled out
  while(!logical_equal(NUM_LEDS, wipe, have)){
      //  while(millis() < end){
    fadeToBlackBy(leds, NUM_LEDS, 75);
    for(i = 0; i < n_drop; i++){
      if(millis() > end && wipe[XY(cols[i], rows[i])]){
	if(random(0, 3) == 0){
	  have[XY(cols[i], rows[i])] = true;
	}
      }
      
      if(random(0, 16) == 0){ // pause at random times
	pause[i] = random(6, 9); // for random duration
      }
      if(pause[i] == 0){
	rows[i]++;
      }
      else{
	pause[i]--; 
      }
      if(rows[i] > MatrixHeight - 1){
	if(n_drop > n_need){
	  for(j = i; j < n_drop; j++){ // slide drops down by one
	    rows[j] = rows[j + 1];
	    cols[j] = cols[j + 1];
	  }
	  n_drop--;
	  Serial.print("n_drop:");
	  Serial.println(n_drop);
	}
	else{
	  rows[i] = -random(0, MatrixHeight);
	  cols[i] = random(0, MatrixWidth);
	}
      }
      if(0 <= rows[i] && rows[i] <  MatrixHeight){
	leds[XY(cols[i], rows[i])] = color;
      }
    }

    for(int ii = 0; ii < NUM_LEDS; ii++){
      if(have[ii]){
	leds[ii] = CRGB::Blue;
      }
    }
    FastLED.show();
    delay(75);
  }
  for(int ii=0; ii< MatrixHeight * 10; ii++){
      //  while(millis() < end){
    fadeToBlackBy(leds, NUM_LEDS, 75);
    for(i = 0; i < n_drop; i++){
      rows[i]++;
      if(0 <= rows[i] && rows[i] <  MatrixHeight){
	leds[XY(cols[i], rows[i])] = color;
      }
    }
    for(int ii = 0; ii < NUM_LEDS; ii++){
      if(have[ii]){
	leds[ii] = CRGB::Blue;
      }
    }
    FastLED.show();
    delay(75);
  }

  // fade to green
  delay(1000);
  
  
  
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
    rainbow();
    apply_mask(tmp);
    FastLED.show();
    theta += dtheta;
  }
}

void wipe_around_transition(uint16_t last_time_inc, uint16_t time_inc){
  rainbow();
  fillMask(mask, false);
  get_time_display(mask, last_time_inc);
  apply_mask(mask);
  wipe_around(ON);
  delay(10);

  fillMask(mask, false);
  get_time_display(mask, time_inc);
  apply_mask(mask);
  wipe_around(OFF);
  delay(10);
}
void wipe_down(bool val){
  int col, row;
  bool tmp[NUM_LEDS];
  
  fillMask(wipe, !val);
  for(row=0; row < MatrixHeight; row++){
    for(col = MatrixWidth - 1; col >= 0; col--){
      setPixelMask(wipe, row, col, val);
    }
    logical_or(NUM_LEDS, wipe, mask, tmp);
    rainbow();
    apply_mask(tmp);
    FastLED.show();
    delay(20);
  }
}

void wipe_up(bool val){
  int col, row;
  bool tmp[NUM_LEDS];
  
  fillMask(wipe, !val);
  for(row = MatrixHeight - 1; row >= 0; row--){
    for(col = MatrixWidth - 1; col >= 0; col--){
      setPixelMask(wipe, row, col, val);
    }
    logical_or(NUM_LEDS, wipe, mask, tmp);
    rainbow();
    apply_mask(tmp);
    FastLED.show();
    delay(20);
  }
}

void wipe_left(bool val){
  int col, row;
  bool tmp[NUM_LEDS];
  
  fillMask(wipe, !val);
  for(col = MatrixWidth - 1; col >= 0; col--){
    for(row=0; row < MatrixHeight; row++){
      setPixelMask(wipe, row, col, val);
    }
    logical_or(NUM_LEDS, wipe, mask, tmp);
    rainbow();
    apply_mask(tmp);
    FastLED.show();
    delay(10);
  }
}

void next_display(){
  Serial.println("Next Display");
  String name = CurrentDisplay_p->name;
  Serial.println(name);
  int next_display_i = CurrentDisplay_p->id + 1;
  next_display_i %= N_DISPLAY;
  CurrentDisplay_p = Display_ps[next_display_i];
  CurrentDisplay_p->init();
  Serial.println(CurrentDisplay_p->name);
}

void desk_clock(){
  bool tmp_d[NUM_LEDS];
  uint8_t word[3];                // will store start_x, start_y, length of word
  time_t spm;                     // seconds past midnight
  uint16_t time_inc;              // 5-minute time increment are we in
  uint8_t minute_hack_inc;        // minute hack

  if(millis() - last_update_ms > UPDATE_INTERVAL_MS){
    timeClient.update();
    handleTimeSeconds(timeClient.getEpochTime());
    last_update_ms = millis();
  }
  if(millis() - last_io_update_ms > UPDATE_IO_MS){
    io.run();
    last_io_update_ms - millis();
  }

  spm = current_time % 86400;
  time_inc = spm / 300;
  minute_hack_inc = (spm % 300) / (300. / float(n_minute_state));
  
  if(time_inc != last_time_inc){
    CurrentDisplay_p->transition(last_time_inc, time_inc);
    last_time_inc = time_inc;
  }
  
  fillMask(mask, false);
  get_time_display(mask, time_inc);
  CurrentDisplay_p->background();
  apply_mask(mask);
  FastLED.show();
  return;
}

void Normal_loop(void) {
}

void JustUpdateTime(bool *mask, uint16_t time_inc){
  fillMask(mask, false);
  
  // read display for next time incement
  get_time_display(mask, time_inc);

  apply_mask(mask);
  FastLED.show();
}

/*
 * DISPLAYS: 288 32 bit settings.  one for each 5-minute period.  up to 32 words per setting.
 * To turn on word one in ith display: 10000000000000000000000000000000
 *
 * WORDS:  3 * n + 1 bytes.  first byte is # words followed by ordered words.
 *                                    x         y       l
 *        Each word is defined by startcol, startrow, len
 */

void getword(int i, uint8_t* out){
  out[0] = pgm_read_byte(WORDS + 3 * i + 1);
  out[1] = pgm_read_byte(WORDS + 3 * i + 2);
  out[2] = pgm_read_byte(WORDS + 3 * i + 3);
}


// read the ith minute hack from program memory
uint32_t getminutebits(uint8_t i){
  return pgm_read_dword(MINUTES_HACK + i);
}

void get_time_display(bool* mask, int i){
  uint8_t bits;     // holds the on off state for 8 words at a time
  uint8_t word[3];  // start columm, start row, length of the current word

  for(uint8_t j = 0; j < n_byte_per_display; j++){ // j is a byte index 
    
    // read the state for the next set of 8 words
    bits = pgm_read_byte(DISPLAYS + 1 + (i * n_byte_per_display) + j);
    for(uint8_t k = 0; k < 8; k++){                     // k is a bit index
      if((bits >> k) & 1){                              // check to see if word is on or off
	getword(j * 8 + k, word);                       // if on, read location and length
	for(int m=word[0]; m < word[0] + word[2]; m++){ // and display it
	  setPixelMask(mask, word[1], m, true);
	}
      }
    }
  } 
}

/*
 * minutes_hack -- prepare display to show ith minute hack intrement
 *   i -- minute hack index
 * out -- display to prepare
 */
void minutes_hack(uint8_t i, bool *out){
  uint8_t col, row, led_num; // position variables for a minute hack LED
  boolean state;             // on/off state variable for a n minute hack LED
  
  // bits is a bit vector that indicates the on/off state for this time incriment i
  uint32_t bits = getminutebits(i);
  for(uint8_t j = 0; j < n_minute_led; j++){      // j is a minute-hack LED index

    // read LED location:
    //     skip first two bytes that contain values for number of minute hack states and number of minute hack leds
    led_num = pgm_read_byte(MINUTE_LEDS + 2 + j); 

    // pull location information for this LED
    col =   (led_num & 0b00001111);
    row =   (led_num & 0b01110000) >> 4; // msb is ignored
    state = (bits >> j) & 1;
    if(state){
      out[col] |= 1<<row;
    }
    else{
      out[col] &= (~(1<<row));
    }
  } 
}

