#include "config/Config.h"
#include "input/InputManager.h"
#include "led/LEDManager.h"
#include "display/DisplayManager.h"
#include "time/TimeManager.h"
#include <WiFi.h>

InputManager inputManager(encoderButton_PIN, encoderDT_PIN, encoderCLK_PIN);
LEDManager ledManager(LED_PIN, NUM_LEDS);
DisplayManager displayManager;
TimeManager timeManager;

unsigned long lastBrightnessChangeTime = 0;
unsigned long lastModeChangeTime = 0;
unsigned long lastWifiCheckTime = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000; // Check WiFi every 5 seconds

void setup() {
    Serial.begin(9600);
    
    // Initialize components
    inputManager.begin();
    ledManager.begin();
    displayManager.begin();
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) { // Try for 10 seconds
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
    } else {
        Serial.println("WiFi connection failed");
    }
    
    timeManager.begin();
}

void loop() {
    inputManager.update();
    
    // Periodically check WiFi status
    if (millis() - lastWifiCheckTime > WIFI_CHECK_INTERVAL) {
        timeManager.updateLocalTime();
        lastWifiCheckTime = millis();
    }
    
    // Check for encoder button press
    if (inputManager.isEncoderButtonPressed()) {
        ledManager.setMode((ledManager.getMode() + 1) % 2);
        lastModeChangeTime = millis();
    }

    // Check for encoder button release
    if (inputManager.isEncoderButtonReleased()) {
        // Do nothing, wait for the next button press
    }

    // Check for encoder rotation
    int encoderDelta = inputManager.getEncoderDelta();
    if (encoderDelta != 0) {
        int newBrightness = constrain(ledManager.getBrightness() + encoderDelta, 0, 255);
        ledManager.setBrightness(newBrightness);
        lastBrightnessChangeTime = millis();
    }

    // Update components
    ledManager.update();
    displayManager.update(ledManager.getMode(), 
                         ledManager.getBrightness(), 
                         lastModeChangeTime, 
                         lastBrightnessChangeTime,
                         timeManager.getStatusMessage());
}