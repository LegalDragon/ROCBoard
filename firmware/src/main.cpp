// ROCBoard — Phase 1 firmware
// Blinks the single onboard WS2812B LED: RED → GREEN → BLUE → OFF (500ms each)
// Uses millis() for non-blocking timing. NO delay() calls.

#include <Arduino.h>
#include "LedDriver.h"
#include "../include/HardwareProfiles.h"

static LedDriver ledDriver;

// Blink state machine
enum class BlinkState { RED, GREEN, BLUE, OFF };
static BlinkState blinkState = BlinkState::RED;
static unsigned long lastStateChange = 0;
static const unsigned long STATE_INTERVAL_MS = 500;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) { /* wait up to 3s for serial */ }

    Serial.println("ROCBoard Phase 1 starting...");
    Serial.print("Profile: ");
    Serial.println(PROFILE_1LED.name);
    Serial.print("LED count: ");
    Serial.println(PROFILE_1LED.ledCount);
    Serial.print("LED pin: ");
    Serial.println(PROFILE_1LED.ledPin);

    ledDriver.init(PROFILE_1LED);
    lastStateChange = millis();

    Serial.println("LED driver initialized. Beginning blink cycle R→G→B→OFF");
}

void loop() {
    unsigned long now = millis();

    if (now - lastStateChange < STATE_INTERVAL_MS) {
        return; // Not time to transition yet
    }

    lastStateChange = now;

    switch (blinkState) {
        case BlinkState::RED:
            ledDriver.setPixel(0, 255, 0, 0);
            ledDriver.show();
            Serial.println("[LED] RED");
            blinkState = BlinkState::GREEN;
            break;

        case BlinkState::GREEN:
            ledDriver.setPixel(0, 0, 255, 0);
            ledDriver.show();
            Serial.println("[LED] GREEN");
            blinkState = BlinkState::BLUE;
            break;

        case BlinkState::BLUE:
            ledDriver.setPixel(0, 0, 0, 255);
            ledDriver.show();
            Serial.println("[LED] BLUE");
            blinkState = BlinkState::OFF;
            break;

        case BlinkState::OFF:
            ledDriver.clear();
            Serial.println("[LED] OFF");
            blinkState = BlinkState::RED;
            break;
    }
}
