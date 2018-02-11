#ifndef DISPLAY_H
#define DISPLAY_H

struct config_t{
  int timezone;
  long alarm;
  int mode;
  uint8_t brightness;
  uint8_t display_idx;
} configuration;

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

void blend_to_color();
void new_display(Display* Display_p);
void blend_to_rainbow();
void set_display(int display_idx);

#endif
