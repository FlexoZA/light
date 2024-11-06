#ifndef TEMP_MANAGER_H
#define TEMP_MANAGER_H

#include <DHT.h>

#define DHTTYPE DHT11

class TempManager {
private:
    DHT* dht;
    float temperature;
    float humidity;
    unsigned long lastReadTime;
    const unsigned long READ_INTERVAL = 2000;
    bool initialized;

public:
    TempManager();
    ~TempManager();
    void begin();
    void update();
    float getTemperature();
    float getHumidity();
    void printReadings();
    bool shouldUpdate();
};

#endif