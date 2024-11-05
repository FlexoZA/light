#pragma once
#include <Arduino.h>

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
    int _lastEncoderDTState;
    int _lastEncoderCLKState;
    int _encoderPosition;
    unsigned long _lastDebounceTime;
    void checkEncoderButton();
    void checkEncoder();
};