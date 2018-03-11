#include "Faceplate.h"

Faceplate::Faceplate(uint8_t *_words,
		     uint8_t *_displays,
		     uint32_t *_minute_leds,
		     uint32_t *_minutes_hack){
  words = _words;
  displays = _displays;
  minute_leds = _minute_leds;
  minutes_hack = _minutes_hack;
}

void Faceplate::setup(uint8_t _width, uint8_t _height, uint16_t (*_XY_p)(uint8_t col, uint8_t row)){
  width = _width;
  height = _height;
  num_leds = width * height;
  XY_p = _XY_p;
}
void Faceplate::setMask(bool* mask, uint8_t row, uint8_t col, bool val){
  if(row >= height){
  }
  else if(col >= width){
  }
  else{
    uint16_t pos = XY_p(col, row);
    if(pos < num_leds){
      mask[pos] = val;
    }
  }
}

void Faceplate::maskTime(uint32_t tm, bool* mask){
  uint8_t bits;     // holds the on off state for 8 words at a time
  uint8_t word[3];  // start columm, start row, length of the current word
  int time_inc;

  uint8_t n_byte_per_display = displays[0];
  
  time_inc = ((tm % 86400) / 300) % 288;
  for(uint8_t j = 0; j < n_byte_per_display; j++){ // j is a byte index 
    
    // read the state for the next set of 8 words
    bits = displays[1 + (time_inc * n_byte_per_display) + j];
    for(uint8_t k = 0; k < 8; k++){                     // k is a bit index
      if((bits >> k) & 1){                              // check to see if word is on or off
	getword(j * 8 + k, word);                       // if on, read location and length
	for(int m=word[0]; m < word[0] + word[2]; m++){ // and display it
	  setMask(mask, word[1], m, true);
	}
      }
    }
  } 
}

void Faceplate::getword(int i, uint8_t* out){
  out[0] = words[3 * i + 1];
  out[1] = words[3 * i + 2];
  out[2] = words[3 * i + 3];
}


