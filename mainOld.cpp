#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"
#include <WiFi.h>

// LED Configuration
#define LED_PIN 4
#define BUTTON_PIN 5
#define POT_PIN 34
#define NUM_LEDS 16

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LED Control Variables
int mode = 0; // 0 for static color, 1 for moving rainbow
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
int lastButtonState = HIGH;
int buttonState = HIGH;
uint16_t hue = 0;
unsigned long lastUpdateTime = 0;
const long updateInterval = 20;

// Display timing variables
unsigned long lastBrightnessChangeTime = 0;
unsigned long lastModeChangeTime = 0;
const unsigned long brightnessDisplayDuration = 1000; // Display brightness for 1 second
const unsigned long modeDisplayDuration = 2000;      // Display mode for 2 seconds
int currentBrightness = 0;

// Time keeping
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Connect to Wi-Fi (you need to add your Wi-Fi credentials)
  WiFi.begin("flexoZA", "Chr0846400936#");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
   }
   Serial.println("WiFi connected");

  // Initialize and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  checkButton();
  updateBrightness();
  updateLEDs();
  updateOLEDDisplay();
}

void checkButton() {
    int reading = digitalRead(BUTTON_PIN);
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;
            if (buttonState == LOW) {
                mode = (mode + 1) % 2;
                lastModeChangeTime = millis(); // Record when mode was changed
                Serial.println("Mode changed to: " + String(mode));
            }
        }
    }
    
    lastButtonState = reading;
}

void updateBrightness() {
  int potValue = analogRead(POT_PIN);
  int newBrightness = map(potValue, 0, 4095, 255, 0);
  
  if (abs(newBrightness - currentBrightness) > 2) {  // Add a small threshold
    currentBrightness = newBrightness;
    pixels.setBrightness(currentBrightness);
    lastBrightnessChangeTime = millis();
  }
}

void updateLEDs() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    mode == 0 ? staticColor() : movingRainbow();
  }
}

void staticColor() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 240));
  }
  pixels.show();
}

void movingRainbow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int pixelHue = (hue + (i * 65536L / NUM_LEDS)) % 65536;
    pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  }
  pixels.show();
  hue = (hue + 256) % 65536;
}


void updateOLEDDisplay() {
    display.clearDisplay();
    
    // Check display priority: Mode > Brightness > Time
    if (millis() - lastModeChangeTime < modeDisplayDuration) {
        displayMode();
    } else if (millis() - lastBrightnessChangeTime < brightnessDisplayDuration) {
        displayBrightnessBar();
    } else {
        displayTime();
    }
    
    display.display();
}

void displayBrightnessBar() {
  int barWidth = map(currentBrightness, 0, 255, 0, SCREEN_WIDTH - 4);
  int percentage = map(currentBrightness, 0, 255, 0, 100);
  
  display.drawRect(0, 10, SCREEN_WIDTH, 20, SSD1306_WHITE);
  display.fillRect(2, 12, barWidth, 16, SSD1306_WHITE);
  
  display.setCursor((SCREEN_WIDTH - 24) / 2, 40);
  display.print(percentage);
  display.print("%");
}


void displayMode() {
    display.setTextSize(2);
    
    // Calculate text width and position
    int16_t x1, y1;
    uint16_t w, h;
    
    if (mode == 0) {
        // Get the width of "Static" text
        display.getTextBounds("Static", 0, 0, &x1, &y1, &w, &h);
        // Center horizontally: (screen width - text width) / 2
        display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
        display.println("Static");
    } else {
        // Get the width of "Rainbow" text
        display.getTextBounds("Rainbow", 0, 0, &x1, &y1, &w, &h);
        // Center horizontally: (screen width - text width) / 2
        display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
        display.println("Rainbow");
    }
}


void displayTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    
    char timeString[9];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
    
    display.setTextSize(2);
    display.setCursor((SCREEN_WIDTH - 12 * 6) / 2, (SCREEN_HEIGHT - 16) / 2);
    display.print(timeString);
}
