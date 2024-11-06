#include "time/TimeManager.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7200;     // Change this according to your timezone (e.g., 3600 for GMT+1)
const int daylightOffset_sec = 7200;  // Change if needed for daylight savings

void TimeManager::begin() {
    _lastWifiStatus = WL_CONNECTED;  // Initialize status
    if (WiFi.status() == WL_CONNECTED) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        updateLocalTime();  // Initial time sync
    }
}

bool TimeManager::updateLocalTime() {
    if (WiFi.status() != WL_CONNECTED) {
        _lastWifiStatus = WL_DISCONNECTED;
        return false;
    }
    
    _lastWifiStatus = WL_CONNECTED;
    if (!getLocalTime(&_timeinfo)) {
        Serial.println("Failed to obtain time");
        return false;
    }
    return true;
}

String TimeManager::getTimeString() {
    if (_lastWifiStatus != WL_CONNECTED) {
        return "WiFi Disconnected";
    }
    
    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &_timeinfo);
    return String(timeStringBuff);
}

bool TimeManager::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String TimeManager::getStatusMessage() {
    switch (WiFi.status()) {
        case WL_CONNECTED:
            return "";  // No error message when connected
        case WL_DISCONNECTED:
            return "WiFi Disconnected";
        case WL_CONNECT_FAILED:
            return "WiFi Connect Failed";
        case WL_NO_SSID_AVAIL:
            return "WiFi SSID Not Found";
        case WL_IDLE_STATUS:
            return "WiFi Connecting...";
        default:
            return "WiFi Error";
    }
}