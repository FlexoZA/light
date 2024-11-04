#pragma once
#include <Adafruit_NeoPixel.h>

class LEDManager {
public:
    // Constructor takes the pin number and number of LEDs
    LEDManager(int pin, int numLeds);
    
    // Initialize the LED strip
    void begin();
    
    // Update the LED animations
    void update();
    
    // Set the overall brightness (0-255)
    void setBrightness(int brightness);
    
    // Change the current mode
    void setMode(int newMode);
    
    // Get the current mode
    int getMode() const { return _mode; }
    
private:
    Adafruit_NeoPixel _pixels;  // The NeoPixel strip object
    int _mode;                  // Current mode (0 = static, 1 = rainbow)
    uint16_t _hue;             // Current hue for rainbow effect
    unsigned long _lastUpdateTime;  // Time of last update
    
    // LED pattern methods
    void staticColor();        // Static warm white color
    void movingRainbow();      // Moving rainbow effect
};