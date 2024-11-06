#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "time.h"

class TimeManager {
public:
    void begin();
    bool updateLocalTime();
    String getTimeString();
    bool isWiFiConnected();
    String getStatusMessage();
    
private:
    struct tm _timeinfo;
    wl_status_t _lastWifiStatus;
};