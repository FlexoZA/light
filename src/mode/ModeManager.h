#pragma once

#include "../input/InputManager.h"
#include "../led/LEDManager.h"
#include "../fan/FanManager.h"
#include "media/MediaManager.h"

enum class Mode { Light, Fan, Media };

class ModeManager {
public:
    ModeManager(InputManager& inputManager, LEDManager& ledManager, FanManager& fanManager, MediaManager& mediaManager)
        : _inputManager(inputManager),
          _ledManager(ledManager),
          _fanManager(fanManager),
          _mediaManager(mediaManager),
          _currentMode(Mode::Light),
          _lastModeChangeTime(0) {}

    void update() {
        handleModeChange();
        updateCurrentMode();
    }

    Mode getCurrentMode() const { return _currentMode; }

private:
    InputManager& _inputManager;
    LEDManager& _ledManager;
    FanManager& _fanManager;
    MediaManager& _mediaManager;
    Mode _currentMode;
    unsigned long _lastModeChangeTime;
    const unsigned long MODE_CHANGE_TIMEOUT = 5000; // 5 seconds

    void handleModeChange() {
        if (_inputManager.isEncoderButtonPressed()) {
            _currentMode = static_cast<Mode>(static_cast<int>(_currentMode) + 1);
            if (_currentMode == Mode::Media + 1) {
                _currentMode = Mode::Light;
            }
            _lastModeChangeTime = millis();
        }
    }

    void updateCurrentMode() {
        if (millis() - _lastModeChangeTime > MODE_CHANGE_TIMEOUT) {
            _currentMode = Mode::Light;
        }

        switch (_currentMode) {
            case Mode::Light:
                _ledManager.update();
                break;
            case Mode::Fan:
                _fanManager.update();
                break;
            case Mode::Media:
                _mediaManager.update();
                break;
        }
    }
};