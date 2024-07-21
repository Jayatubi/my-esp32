#include <Arduino.h>
#include "Clock.h"
#include "Network.h"

Clock ntpClock;

Clock::Clock()
  : state(idle) {
  time_zone = "CST-8";
  ntp_server = "ntp.aliyun.com";
}

Clock::~Clock() {
}

void Clock::update(unsigned long frame_count) {
  switch (state) {
    case idle:
      if (network.is_ready()) {
        configTzTime(time_zone, ntp_server);
        state = syncing;
      }
      break;
    case syncing: {
      {
        getLocalTime(&timeinfo, 0);
      }
    }
      break;
  }
}

void Clock::resync() {
  state = idle;
}

time_t Clock::get_time() const {
  time_t now;
  time(&now);
  return now;
}

int Clock::get_year() const {
  return 1900 + timeinfo.tm_year;
}

int Clock::get_month() const {
  return timeinfo.tm_mon;
}

int Clock::get_date() const {
  return timeinfo.tm_mday;
}

int Clock::get_day() const {
  return timeinfo.tm_wday;
}

int Clock::get_hours() const {
  return timeinfo.tm_hour;
}

int Clock::get_minutes() const {
  return timeinfo.tm_min;
}

int Clock::get_seconds() const {
  return timeinfo.tm_sec;
}

int Clock::get_state() const {
  return state;
}
