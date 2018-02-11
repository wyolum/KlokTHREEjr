#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <credentials.h>
#include "klok.h"

#include <EEPROM.h>
#include <EEPROMAnything.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <WiFiClientSecure.h>

void setup(){
  Serial.begin(115200);
  delay(100);
  
  thingsboard_setup();
  klok_setup();
}

int my_display_idx = 0;
void loop(){
  thingsboard_loop();
  klok_loop();

  int new_idx;
  new_idx = tb_get_idx();
  if(new_idx != my_display_idx){
    my_display_idx = new_idx;
    set_display(my_display_idx);
  }
}
