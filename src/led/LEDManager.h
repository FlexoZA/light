// LEDManager.h
#pragma once
#include <Adafruit_NeoPixel.h>

class LEDManager {
public:
    LEDManager(int pin, int numLeds);
    void begin();
    void update();
    void setBrightness(int brightness);
    int getBrightness() { return _pixels.getBrightness(); }
    void setMode(int newMode);
    int getMode() { return _mode; }

private:
    Adafruit_NeoPixel _pixels;
    int _mode;
    int _hue;
    unsigned long _lastUpdateTime;
    void staticColor();
    void movingRainbow();
};