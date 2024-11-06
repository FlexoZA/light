#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void update(int mode, int brightness, unsigned long lastModeChange, 
                unsigned long lastBrightnessChange, String wifiStatus = "");
    
private:
    void displayMode(int mode);
    void displayBrightnessBar(int brightness);
    void displayTime();
    void displayWifiStatus(const String& status);
    
    Adafruit_SSD1306 _display;
};