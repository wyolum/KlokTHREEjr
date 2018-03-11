#ifndef FACEPLATE_H
#define FACEPLATE_H

class Faceplate{
  uint8_t *words;
  uint8_t *displays;
  uint32_t *minute_leds;
  uint32_t *minute_hack;

 public:
  Faceplate(uint8_t *words,
	    uint8_t *displays,
	    uint32_t *minute_leds,
	    uint32_t *minutes_hack);
};

Faceplate::Faceplate(uint8_t *words,
		     uint8_t *displays,
		     uint32_t *minute_leds,
		     uint32_t *minutes_hack){
}

#endif
