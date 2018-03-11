#include "Faceplate.h"


class Klok{
 public:
  Klok(Faceplate _faceplate, NTPClient _timeClient);
  void get_time_display(bool* mask, int i);
  Faceplate faceplate;
  NTPClient timeClient;
  uint8_t n_byte_per_display = 4;         // number of bytes used for each 5 minunte time incriment
};

Klok::Klok(Faceplate _faceplate, NTPClient _timeClient){
  faceplate = _faceplate;
  timeClient = _timeClient;
}

void Klok::get_time_display(bool* mask, int i){
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

