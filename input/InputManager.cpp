#include "InputManager.h"
#include "../config/Config.h"

InputManager::InputManager(int buttonPin, int potPin) 
    : _buttonPin(buttonPin), _potPin(potPin), _lastButtonState(HIGH), _buttonState(HIGH) {}

void InputManager::begin() {
    pinMode(_buttonPin, INPUT_PULLUP);
    pinMode(_potPin, INPUT);
}

void InputManager::update() {
    checkButton();
}

void InputManager::checkButton() {
    int reading = digitalRead(_buttonPin);
    if (reading != _lastButtonState) {
        _lastDebounceTime = millis();
    }
    
    if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != _buttonState) {
            _buttonState = reading;
        }
    }
    _lastButtonState = reading;
}

bool InputManager::isButtonPressed() {
    return _buttonState == LOW;
}

int InputManager::getPotValue() {
    return analogRead(_potPin);
}