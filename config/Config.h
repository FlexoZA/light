#pragma once

// Pin Configuration
#define LED_PIN 4
#define BUTTON_PIN 5
#define POT_PIN 34
#define NUM_LEDS 16

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// Time Configuration
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 7200
#define DAYLIGHT_OFFSET_SEC 0

// Timing Constants
#define DEBOUNCE_DELAY 50
#define UPDATE_INTERVAL 20
#define BRIGHTNESS_DISPLAY_DURATION 1000
#define MODE_DISPLAY_DURATION 2000

// WiFi Configuration
#define WIFI_SSID "flexoZA"
#define WIFI_PASSWORD "Chr0846400936#"