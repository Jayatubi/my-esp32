#include <U8g2lib.h>
#include "Network.h"
#include "Clock.h"

#define SPI_CS 5
#define SPI_DC 16
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 64

U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC);

template<class T, size_t N>
constexpr size_t count_of(T (&)[N]) {
  return N;
}


void init_u8g2() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_wqy12_t_chinese3);
  u8g2.setFontDirection(0);
}

namespace kb {
static char kb_rowp[] = {33, 32, 35, 34, 39};
static char kb_colp[] = {13, 12, 14, 27, 26, 25};

void init_matrix() {
  for (char r : kb_rowp) {
    pinMode(r, INPUT);
  }
  for (char c : kb_colp) {
    pinMode(c, OUTPUT);
  }
}

void scan() {
  constexpr int offset_x = 2;
  constexpr int offset_y = 2;
  constexpr int width = 4;
  constexpr int margin = 1;
  constexpr int padding = 1;

  for (int c = 0; c < count_of(kb_colp); c++) {
    digitalWrite(kb_colp[c], HIGH);
    for (int r = 0; r < count_of(kb_rowp); r++) {
      const auto is_high = digitalRead(kb_rowp[r]);
      auto func = is_high ? &U8G2::drawBox : &U8G2::drawFrame;
      (u8g2.*func)(c * (width + margin * 2) + offset_x, r * (width + margin * 2) + offset_y, width, width);
    }
    digitalWrite(kb_colp[c], LOW);
  }
}
}

void setup() {
  Serial.begin(115200);

  init_u8g2();
  kb::init_matrix();
}

void banner() {
  const auto line_height = u8g2.getAscent() + 2;
  u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight());

  u8g2.setCursor(40, line_height);
  u8g2.printf("SSID: %s", network.get_ssid());
  u8g2.setCursor(40, line_height * 2);
  u8g2.printf("IPv4: %s", network.get_ipaddress().toString().c_str());
  u8g2.setCursor(40, line_height * 3);
  u8g2.printf("Time: %04d/%02d/%02d %02d:%02d:%02d",
              ntpClock.get_year(), ntpClock.get_month() + 1, ntpClock.get_date(),
              ntpClock.get_hours(), ntpClock.get_minutes(), ntpClock.get_seconds());
}

void loop() {
  static unsigned long frame_count = 0;
  network.update(frame_count);
  ntpClock.update(frame_count);

  u8g2.clearBuffer();
  kb::scan();
  banner();
  u8g2.sendBuffer();

  frame_count++;
}
