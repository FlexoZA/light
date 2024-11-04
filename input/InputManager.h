#pragma once
#include <Arduino.h>

class InputManager {
public:
    InputManager(int buttonPin, int potPin);
    void begin();
    void update();
    bool isButtonPressed();
    int getPotValue();
    
private:
    int _buttonPin;
    int _potPin;
    int _lastButtonState;
    int _buttonState;
    unsigned long _lastDebounceTime;
    void checkButton();
};