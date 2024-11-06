#include "LEDManager.h"
#include "../config/Config.h"

LEDManager::LEDManager(int pin, int numLeds) 
    : _pixels(numLeds, pin, NEO_GRB + NEO_KHZ800), _mode(0), _hue(0), _lastUpdateTime(0) {}

void LEDManager::begin() {
    _pixels.begin();
    _pixels.show(); // Initialize all pixels to 'off'
}

void LEDManager::update() {
    unsigned long currentTime = millis();
    if (currentTime - _lastUpdateTime >= UPDATE_INTERVAL) {
        _lastUpdateTime = currentTime;
        _mode == 0 ? staticColor() : movingRainbow();
    }
}

void LEDManager::setBrightness(int brightness) {
    brightness = constrain(brightness, 0, 255); // Ensure brightness is within valid range
    _pixels.setBrightness(brightness);
    _pixels.show();
}

void LEDManager::setMode(int newMode) {
    _mode = newMode;
    _hue = 0; // Reset hue when mode changes
    update(); // Immediately update the LEDs when mode changes
}

void LEDManager::staticColor() {
    for (int i = 0; i < _pixels.numPixels(); i++) {
        _pixels.setPixelColor(i, _pixels.Color(255, 255, 240)); // Warm white color
    }
    _pixels.show();
}

void LEDManager::movingRainbow() {
    for (int i = 0; i < _pixels.numPixels(); i++) {
        // Calculate the hue for this pixel. Each pixel gets a different hue based on its position
        int pixelHue = (_hue + (i * 65536L / _pixels.numPixels())) % 65536;
        
        // Convert the hue to RGB color and set the pixel
        _pixels.setPixelColor(i, _pixels.gamma32(_pixels.ColorHSV(pixelHue)));
    }
    
    // Show the updated pixels
    _pixels.show();
    
    // Increment the base hue for the next update
    // The smaller this increment, the slower the rainbow moves
    _hue = (_hue + 256) % 65536;
}