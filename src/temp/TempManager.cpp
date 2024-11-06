#include "TempManager.h"
#include "../config/Config.h"

TempManager::TempManager() {
    Serial.println("TempManager: Constructing...");
    dht = new DHT(DHT_PIN, DHTTYPE);
    temperature = 0.0f;
    humidity = 0.0f;
    lastReadTime = 0;
    initialized = false;
    Serial.println("TempManager: Construction complete");
}

TempManager::~TempManager() {
    delete dht;
}

void TempManager::begin() {
    Serial.println("TempManager: Initializing DHT11...");
    dht->begin();
    delay(1000); // Give the sensor some time to stabilize
    initialized = true;
    Serial.println("TempManager: DHT11 Initialized on pin " + String(DHT_PIN));
}

bool TempManager::shouldUpdate() {
    return (millis() - lastReadTime) >= READ_INTERVAL;
}

void TempManager::update() {
    if (!initialized) {
        Serial.println("TempManager: Error - Not initialized!");
        return;
    }

    if (!shouldUpdate()) {
        return;
    }

    Serial.println("TempManager: Reading sensor...");
    
    // Reading temperature and humidity
    humidity = dht->readHumidity();
    temperature = dht->readTemperature();

    // Check if any reads failed
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("TempManager: Failed to read from DHT sensor!");
        Serial.println("TempManager: Please check your wiring and pull-up resistor");
        return;
    }

    Serial.println("TempManager: Successful reading");
    lastReadTime = millis();
    printReadings();
}

float TempManager::getTemperature() {
    return temperature;
}

float TempManager::getHumidity() {
    return humidity;
}

void TempManager::printReadings() {
    Serial.println("----------------------------------------");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C  |  Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.println("----------------------------------------");
}