#ifndef TEXTURES_H
#define TEXTURES_H

#include <FastLED.h>
#include <MatrixMaps.h>

#define MatrixHeight 8
#define MatrixWidth 16
void rainbow(CRGB *leds, uint16_t current_time, uint16_t (*XY)(uint8_t col, uint8_t row));

#endif
