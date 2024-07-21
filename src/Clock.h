#pragma once

class Clock {
 public:
  Clock();

  ~Clock();

  void update(unsigned long frame_count);

  void resync();

  time_t get_time() const;

  int get_state() const;

  int get_year() const;

  int get_month() const;

  int get_date() const;

  int get_day() const;

  int get_hours() const;

  int get_minutes() const;

  int get_seconds() const;

 private:
  enum State {
    idle,
    syncing,
  };

  State state;
  tm timeinfo;

  const char* time_zone;
  const char* ntp_server;
};

extern Clock ntpClock;
