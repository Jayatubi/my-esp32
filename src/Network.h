#pragma once

#include <IPAddress.h>

class Network {
public:
    Network();

    void update(unsigned long frame_count);

    const char *get_ssid() const;
    IPAddress get_ipaddress() const;
    bool is_ready() const;
private:
    enum State {
        idle,
        connecting,
        ready,
        fail
    };
    State state;
    const char *ssid;
    const char *password;
};

extern Network network;
