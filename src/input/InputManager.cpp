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
      _lastDebounceTime(0) {}

void InputManager::begin() {
    pinMode(_encoderButtonPin, INPUT_PULLUP);
    pinMode(_encoderDTPin, INPUT_PULLUP);
    pinMode(_encoderCLKPin, INPUT_PULLUP);

    // Initialize the rotary encoder with the ESP32Encoder library
    encoder.attachFullQuad(_encoderCLKPin, _encoderDTPin); // Attach the CLK and DT pins
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
    // Read the current encoder position
    long position = encoder.getCount(); // Get the current position of the encoder

    // Update the position to change the direction
    // Make the clockwise rotation result in an increased value (more)
    // And the counterclockwise rotation result in a decreased value (less)
    if (position > 0) {
        Serial.print("Encoder Position: ");
        Serial.println(position);  // Clockwise (more)
    } else if (position < 0) {
        Serial.print("Encoder Position: ");
        Serial.println(position);  // Counterclockwise (less)
    }
}

int InputManager::getEncoderDelta() {
    long position = encoder.getCount(); // Get the current position of the encoder
    int delta = -position;  // The delta is the position change
    encoder.clearCount();  // Clear the count after reading
    return delta;
}