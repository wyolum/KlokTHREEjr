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
}

void NTPClock::setup(NTPClient *_timeClient){
  this->timeClient = _timeClient;
  this->timeClient->setTimeOffset(-240 * 60);
  this->timeClient->begin();
}
uint32_t NTPClock::now(){
  this->timeClient->update();
  return this->timeClient->getEpochTime();
}

