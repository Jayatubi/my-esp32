#include "Network.h"
#include <WiFi.h>

Network network;

Network::Network() {
  state = idle;
  ssid = "Jay Home";
  password = "xxcckkll";
}

void Network::update(unsigned long frame_count) {
  const auto status = WiFiClass::status();
  switch (state) {
    case idle:WiFiClass::mode(WIFI_STA); //設置WiFi模式
      WiFi.begin(ssid, password);
      state = connecting;
      break;
    case connecting: {
      if (status == WL_CONNECTED) {
        state = ready;
      }
      if (status == WL_CONNECT_FAILED) {
        state = fail;
      }
    }
      break;
    case ready: {
      if (status == WL_CONNECTION_LOST) {
        state = idle;
      }
      break;
    }
    default:break;
  }
}

const char *Network::get_ssid() const {
  return ssid;
}

IPAddress Network::get_ipaddress() const {
  IPAddress ip;

  if (state == ready) {
    ip = WiFi.localIP();
  }

  return ip;
}

bool Network::is_ready() const {
  return state == ready;
}
