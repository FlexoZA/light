#pragma once
#include <Arduino.h>
#include <ESP32Encoder.h>  // Include the ESP32Encoder library

class InputManager {
public:
    InputManager(int encoderButtonPin, int encoderDTPin, int encoderCLKPin);
    void begin();
    void update();
    bool isEncoderButtonPressed();
    bool isEncoderButtonReleased();
    int getEncoderDelta();

private:
    int _encoderButtonPin;
    int _encoderDTPin;
    int _encoderCLKPin;
    int _lastEncoderButtonState;
    int _encoderButtonState;
    bool _wasEncoderButtonPressed;
    bool _allowEncoderButtonPress;
    unsigned long _lastDebounceTime;
    ESP32Encoder encoder;  // Encoder object from ESP32Encoder library

    void checkEncoderButton();
    void checkEncoder();
};