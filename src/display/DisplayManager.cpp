#include "DisplayManager.h"
#include "../config/Config.h"

DisplayManager::DisplayManager() 
    : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void DisplayManager::begin() {
    if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);
}

void DisplayManager::update(int mode, int brightness, unsigned long lastModeChange, 
                          unsigned long lastBrightnessChange, String wifiStatus) {
    _display.clearDisplay();
    
    // If there's a WiFi error, display it
    if (wifiStatus != "") {
        displayWifiStatus(wifiStatus);
    }
    // Otherwise show normal display hierarchy
    else if (millis() - lastModeChange < MODE_DISPLAY_DURATION) {
        displayMode(mode);
    } else if (millis() - lastBrightnessChange < BRIGHTNESS_DISPLAY_DURATION) {
        displayBrightnessBar(brightness);
    } else {
        displayTime();
    }
    
    _display.display();
}

void DisplayManager::displayMode(int mode) {
    _display.setTextSize(2);
    
    // Calculate text width and position
    int16_t x1, y1;
    uint16_t w, h;
    const char* text = mode == 0 ? "Static" : "Rainbow";
    
    // Get the width of text
    _display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    
    // Center the text horizontally and vertically
    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;
    
    _display.setCursor(x, y);
    _display.println(text);
}

void DisplayManager::displayBrightnessBar(int brightness) {
    // Draw the frame for the progress bar
    int barWidth = map(brightness, 0, 255, 0, SCREEN_WIDTH - 4);
    int percentage = map(brightness, 0, 255, 0, 100);
    
    // Draw outer rectangle
    _display.drawRect(0, 10, SCREEN_WIDTH, 20, SSD1306_WHITE);
    
    // Fill progress bar
    _display.fillRect(2, 12, barWidth, 16, SSD1306_WHITE);
    
    // Display percentage
    _display.setTextSize(1);
    
    // Calculate width of percentage text
    int16_t x1, y1;
    uint16_t w, h;
    String percentText = String(percentage) + "%";
    _display.getTextBounds(percentText.c_str(), 0, 0, &x1, &y1, &w, &h);
    
    // Center the percentage text
    _display.setCursor((SCREEN_WIDTH - w) / 2, 40);
    _display.print(percentText);
}

void DisplayManager::displayTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    
    char timeString[9];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
    
    // Calculate text width and position for centering
    _display.setTextSize(2);
    int16_t x1, y1;
    uint16_t w, h;
    _display.getTextBounds(timeString, 0, 0, &x1, &y1, &w, &h);
    
    // Center the text
    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;
    
    _display.setCursor(x, y);
    _display.print(timeString);
}

void DisplayManager::displayWifiStatus(const String& status) {
    _display.setTextSize(1);
    
    // Calculate text width and position for centering
    int16_t x1, y1;
    uint16_t w, h;
    _display.getTextBounds(status.c_str(), 0, 0, &x1, &y1, &w, &h);
    
    // Center the text horizontally and vertically
    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;
    
    _display.setCursor(x, y);
    _display.println(status);
}