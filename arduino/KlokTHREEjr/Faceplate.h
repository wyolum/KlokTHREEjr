#ifndef FACEPLATE_H
#define FACEPLATE_H
#include "english_v3.h"

class Faceplate{
  uint8_t *words;
  uint8_t *displays;
  uint32_t *minute_leds;
  uint32_t *minute_hack;

 public:
  Faceplate(uint8_t *_words,
	    uint8_t *_displays,
	    uint32_t *_minute_leds,
	    uint32_t *_minute_hack);
};

#include "english_v3.h"
Faceplate english(WORDS, DISPLAYS, MINUTE_LEDS, MINUTES_HACK);

#endif
