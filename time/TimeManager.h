#pragma once
#include <Arduino.h>
#include "time.h"

class TimeManager {
public:
    void begin();
    bool updateLocalTime();
    String getTimeString();
    
private:
    struct tm _timeinfo;
};