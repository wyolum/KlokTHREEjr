#include <credentials.h>
//#include <ArduinoHttpClient.h>
#include <AdafruitIO_WiFi.h>
#include <EEPROM.h>
#include <EEPROMAnything.h>
#include "config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FastLED.h>

/* ---- credentials.h ----
char ssid[] = "XXX";  // your network SSID (name)
char pass[] = "YYY";  // your network password
*/

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

struct config_t
{
  int timezone;
  long alarm;
  int mode;
  uint8_t brightness;
} configuration;

// language constants
uint8_t n_minute_led;               // number of minute hack leds
uint8_t n_minute_state;             // number of minute hack states to cycle through
uint8_t n_byte_per_display = 4;         // number of bytes used for each 5 minunte time incriment

uint8_t display_idx;
uint8_t last_min_hack_inc = 0;
uint16_t last_time_inc = 289;

/* The display memory buffer is larger than physical display for screen 
 * staging and fast display.
 */
const uint8_t N_DISPLAY = 3;
const uint8_t MAX_BRIGHTNESS = 50;
#define ULTIM8x16
#include <MatrixMaps.h>


// How many leds are in the strip?
#define N_BOARD 2
#define NUM_LEDS 64 * N_BOARD
bool mask[N_DISPLAY * NUM_LEDS];

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
  FastLED.setBrightness(8);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  io.connect();
  Serial.print("Wait for IO connect.");
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(io.statusText());Serial.println("Connected to IO");

  timezone->onMessage(handleTimezone);
  brightness->onMessage(handleBrightness);

  EEPROM.begin(512);
  loadSettings();
  timeClient.setTimeOffset(configuration.timezone * 60);
  timeClient.begin();
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
    if (new_brightness != configuration.brightness && 0 <= new_brightness && new_brightness <= MAX_BRIGHTNESS){
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

const struct CRGB color = CRGB::White;
int count = 0;

bool FLIP_DISPLAY = true;
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
  for( int row = 0; row < MatrixHeight; row++) {
    for( int col = 0; col < MatrixWidth; col++) {
      dy = (row - 4) * 2;
      dx = col - 8;
      dist = sqrt(dx * dx + dy * dy);
      i = XY(col, row);
      //hsv.hue =  ((int)(dist * 16) - count) % 256;
      hsv.hue =  (count + MatrixWidth * row + col) % 256;
      leds[i] = hsv;
    }
  }
  // Show the leds (only one of which is set to white, from above)
  //delay(100);
}

void fillMask(bool b){
  fillMask(b, 0, NUM_LEDS);
}

void fillMask(bool b, int start, int stop){
  for(int i = start; i < stop && i < NUM_LEDS; i++){
    mask[i] = b;
  }
}
void setPixelMask(uint8_t row, uint8_t col, bool b){
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

void apply_mask(){
  uint16_t b, k;
  for(uint16_t i=0; i < NUM_LEDS; i++){
    if(!mask[i]){
      leds[i] = CRGB::Black;
    }
  }
}

void minutes_hack(uint8_t i, bool *out);
void getdisplay(int i);

const int32_t UPDATE_INTERVAL_MS = 1000;
int32_t last_update_ms= 0;

void loop(){
  
  io.run();

  if(millis() - last_update_ms > UPDATE_INTERVAL_MS){
    timeClient.update();
    handleTimeSeconds(timeClient.getEpochTime());
    last_update_ms = millis();
  }

  rainbow();
  fillMask(false);
  getdisplay((current_time / 300) % 288);
  count++;
  apply_mask();
  FastLED.show();
}

void Normal_loop(void) {
  time_t standard_time = 0;

  uint8_t word[3];                // will store start_x, start_y, length of word
  time_t spm = standard_time % 86400; // seconds past midnight

#if defined(USE_USA_DST) || defined(USE_EURO_DST)
  if(is_dst(standard_time)){
    spm += 3600;
    spm %= 86400;
  }
#endif

  uint16_t time_inc = spm / 300;  // 5-minute time increment are we in

  // which mininute hack incriment are we on?
  uint8_t minute_hack_inc = (spm % 300) / (300. / float(n_minute_state));

  // if minute hack or time has changed, update the display
  if(minute_hack_inc != last_min_hack_inc || time_inc != last_time_inc){
    
    // prepare other display on change
    display_idx++;
    display_idx %= 2;
    bool* tmp_d = mask + MatrixWidth * MatrixHeight * (display_idx % 2);

    // clear the new display
    for(int ii = 0; ii < MatrixWidth; ii++){
      tmp_d[ii] = 0;
    }

    // read display for next time incement
    getdisplay(time_inc);
  
    // read minutes hack for next time incement
    minutes_hack(minute_hack_inc, tmp_d);
    //c3.fadeto(tmp_d, 32); // 32 fade steps to new display

    last_min_hack_inc = minute_hack_inc;
    last_time_inc = time_inc;
  }
  // Keep active LEDs lit
  // my_refresh(1000);
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

void getdisplay(int i){
  uint8_t bits;     // holds the on off state for 8 words at a time
  uint8_t word[3];  // start columm, start row, length of the current word

  for(uint8_t j = 0; j < n_byte_per_display; j++){ // j is a byte index 
    
    // read the state for the next set of 8 words
    bits = pgm_read_byte(DISPLAYS + 1 + (i * n_byte_per_display) + j);
    for(uint8_t k = 0; k < 8; k++){                     // k is a bit index
      if((bits >> k) & 1){                              // check to see if word is on or off
	getword(j * 8 + k, word);                       // if on, read location and length
	for(int m=word[0]; m < word[0] + word[2]; m++){ // and display it
	  setPixelMask(word[1], m, true);
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

