#include "clock.h"

uint32_t current_time = 0;
Clock::Clock(){
}
DummyClock::DummyClock(){
}
uint32_t DummyClock::now(){
  return current_time++;
}

NTPClock::NTPClock(){
  NTPClient timeClient(ntpUDP, "us.pool.ntp.org", 0, 60000);
  timeClient.setTimeOffset(0);
}
uint32_t NTPClock::now(){
  //timeClient.update();
  return timeClient.getEpochTime();
}

