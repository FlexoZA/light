#include "config/Config.h"
#include "input/InputManager.h"
#include "led/LEDManager.h"
#include "fan/FanManager.h"
#include "media/MediaManager.h"
#include "mode/ModeManager.h"
#include "display/DisplayManager.h"
#include "time/TimeManager.h"
#include "temp/TempManager.h"
#include <WiFi.h>

InputManager inputManager(encoderButton_PIN, encoderDT_PIN, encoderCLK_PIN);
LEDManager ledManager(LED_PIN, NUM_LEDS);
//FanManager fanManager;
//MediaManager mediaManager;
ModeManager modeManager(inputManager, ledManager, fanManager, mediaManager);
DisplayManager displayManager;
TimeManager timeManager;
TempManager tempManager;

unsigned long lastWifiCheckTime = 0;
unsigned long lastTempCheckTime = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000;
const unsigned long TEMP_CHECK_INTERVAL = 2000;

void setup() {
    Serial.begin(9600);
    
    // Initialize components
    inputManager.begin();
    ledManager.begin();
    fanManager.begin();
    mediaManager.begin();
    displayManager.begin();
    tempManager.begin();
    timeManager.begin();
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
    } else {
        Serial.println("WiFi connection failed");
    }
}

void loop() {
    inputManager.update();
    modeManager.update();
    
    // Periodically check WiFi status
    if (millis() - lastWifiCheckTime > WIFI_CHECK_INTERVAL) {
        timeManager.updateLocalTime();
        lastWifiCheckTime = millis();
    }
    
    // Update temperature readings
    if (millis() - lastTempCheckTime > TEMP_CHECK_INTERVAL) {
        tempManager.update();
        lastTempCheckTime = millis();
    }
    
    // Update encoder delta
    int encoderDelta = inputManager.getEncoderDelta();
    if (encoderDelta != 0) {
        switch (modeManager.getCurrentMode()) {
            case Mode::Light:
                ledManager.updateBrightness(encoderDelta);
                break;
            case Mode::Fan:
                fanManager.updateSpeed(encoderDelta);
                break;
            case Mode::Media:
                mediaManager.adjustVolume(encoderDelta);
                break;
        }
    }
    
    // Update components
    ledManager.update();
    fanManager.update();
    mediaManager.update();
    displayManager.update(modeManager.getCurrentMode(),
                         ledManager.getBrightness(),
                         fanManager.getSpeed(),
                         mediaManager.getVolume(),
                         timeManager.getStatusMessage());
}