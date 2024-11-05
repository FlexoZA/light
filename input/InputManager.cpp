#include "InputManager.h"
#include "../config/Config.h"

InputManager::InputManager(int encoderButtonPin, int encoderDTPin, int encoderCLKPin)
    : _encoderButtonPin(encoderButtonPin),
      _encoderDTPin(encoderDTPin),
      _encoderCLKPin(encoderCLKPin),
      _lastEncoderButtonState(HIGH),
      _encoderButtonState(HIGH),
      _wasEncoderButtonPressed(false),
      _allowEncoderButtonPress(true),
      _lastEncoderDTState(HIGH),
      _lastEncoderCLKState(HIGH),
      _encoderPosition(0) {}

void InputManager::begin() {
    pinMode(_encoderButtonPin, INPUT_PULLUP);
    pinMode(_encoderDTPin, INPUT);
    pinMode(_encoderCLKPin, INPUT);
}

void InputManager::update() {
    checkEncoderButton();
    checkEncoder();
}

void InputManager::checkEncoderButton() {
    int reading = digitalRead(_encoderButtonPin);
    if (reading != _lastEncoderButtonState) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != _encoderButtonState) {
            _encoderButtonState = reading;
            if (_encoderButtonState == LOW && _allowEncoderButtonPress) {
                _wasEncoderButtonPressed = true;
                _allowEncoderButtonPress = false;
            } else {
                _wasEncoderButtonPressed = false;
                _allowEncoderButtonPress = true;
            }
        }
    }
    _lastEncoderButtonState = reading;
}

bool InputManager::isEncoderButtonPressed() {
    if (_wasEncoderButtonPressed) {
        _wasEncoderButtonPressed = false;
        return true;
    }
    return false;
}

bool InputManager::isEncoderButtonReleased() {
    return _allowEncoderButtonPress;
}

void InputManager::checkEncoder() {
    int dtState = digitalRead(_encoderDTPin);
    int clkState = digitalRead(_encoderCLKPin);

    if (dtState != _lastEncoderDTState || clkState != _lastEncoderCLKState) {
        if (clkState != _lastEncoderCLKState) {
            if (dtState != _lastEncoderDTState) {
                _encoderPosition += (dtState != clkState) ? 1 : -1;
            }
        }
    }

    _lastEncoderDTState = dtState;
    _lastEncoderCLKState = clkState;
}

int InputManager::getEncoderDelta() {
    int delta = _encoderPosition;
    _encoderPosition = 0;
    return delta;
}