#ifndef CLOCK_H
#define CLOCK_H
#include <stdint.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

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
  WiFiUDP ntpUDP;
  NTPClient timeClient;
 public:
  NTPClock();
  uint32_t now();
};
#endif
