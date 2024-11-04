#include "config/Config.h"
#include "input/InputManager.h"
#include "led/LEDManager.h"
#include "display/DisplayManager.h"
#include "time/TimeManager.h"
#include <WiFi.h>

InputManager inputManager(BUTTON_PIN, POT_PIN);
LEDManager ledManager(LED_PIN, NUM_LEDS);
DisplayManager displayManager;
TimeManager timeManager;

unsigned long lastBrightnessChangeTime = 0;
unsigned long lastModeChangeTime = 0;

void setup() {
    Serial.begin(9600);
    
    // Initialize components
    inputManager.begin();
    ledManager.begin();
    displayManager.begin();
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    
    timeManager.begin();
}

void loop() {
    inputManager.update();
    
    // Check for button press
    if (inputManager.isButtonPressed()) {
        ledManager.setMode((ledManager.getMode() + 1) % 2);
        lastModeChangeTime = millis();
    }
    
    // Update brightness
    int potValue = inputManager.getPotValue();
    int newBrightness = map(potValue, 0, 4095, 255, 0);
    ledManager.setBrightness(newBrightness);
    lastBrightnessChangeTime = millis();
    
    // Update components
    ledManager.update();
    displayManager.update(ledManager.getMode(), 
                         newBrightness, 
                         lastModeChangeTime, 
                         lastBrightnessChangeTime);
}
