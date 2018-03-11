#ifndef CLOCK_H
#define CLOCK_H
#include <stdint.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <RTClib.h>

class Clock{
 public:
  Clock();
  virtual uint32_t now();
};

class DummyClock : public Clock{
 public:
  DummyClock();
  uint32_t now();
};

class NTPClock : public Clock{
 public:
  NTPClock();
  void setup(NTPClient *_timeClient);
  NTPClient *timeClient;
  uint32_t now();
};

class DS3231Clock : public Clock{
  RTC_DS3231 rtc;
 public:
  DS3231Clock();
  void setup();
  uint32_t now();
};

#endif
