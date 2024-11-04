#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void update(int mode, int brightness, unsigned long lastModeChange, unsigned long lastBrightnessChange);
    
private:
    Adafruit_SSD1306 _display;
    void displayMode(int mode);
    void displayBrightnessBar(int brightness);
    void displayTime();
};