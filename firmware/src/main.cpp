// ROCBoard firmware — multi-profile entry point.
// Profile is selected at compile time via HARDWARE_PROFILE build flag.
// Phase 1: single onboard WS2812B blink (GPIO48, PROFILE_1_LED).
// Phase 2: 14-segment single character cycle A→Z→0→9 (GPIO1, PROFILE_14_LED).
// Uses millis() for non-blocking timing. NO delay() calls.

#include <Arduino.h>
#include "LedDriver.h"
#include "DisplayEngine.h"
#include "../include/HardwareProfiles.h"

// ─────────────────────────────────────────────
// PHASE 1: Single RGB LED blink
// ─────────────────────────────────────────────
#if !defined(HARDWARE_PROFILE) || (HARDWARE_PROFILE == PROFILE_1_LED)

static LedDriver ledDriver;

enum class BlinkState { RED, GREEN, BLUE, OFF };
static BlinkState blinkState = BlinkState::RED;
static unsigned long lastStateChange = 0;
static const unsigned long STATE_INTERVAL_MS = 500;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}

    Serial.println("ROCBoard Phase 1 starting...");
    Serial.print("Profile: ");
    Serial.println(PROFILE_1LED.name);
    Serial.print("LED count: ");
    Serial.println(PROFILE_1LED.ledCount);
    Serial.print("LED pin: ");
    Serial.println(PROFILE_1LED.ledPin);

    ledDriver.init(PROFILE_1LED);
    lastStateChange = millis();

    Serial.println("LED driver initialized. Beginning blink cycle R\xe2\x86\x92G\xe2\x86\x92B\xe2\x86\x92OFF");
}

void loop() {
    unsigned long now = millis();
    if (now - lastStateChange < STATE_INTERVAL_MS) return;
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

// ─────────────────────────────────────────────
// PHASE 2: Single 14-segment character A→Z→0→9
// ─────────────────────────────────────────────
#elif HARDWARE_PROFILE == PROFILE_14_LED

static LedDriver    ledDriver;
static DisplayEngine displayEngine;

// 36-char cycle: A-Z then 0-9
static const char CYCLE_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static const uint8_t CYCLE_LEN  = sizeof(CYCLE_CHARS) - 1; // exclude null

static uint8_t        cycleIdx         = 0;
static unsigned long  lastCharChange   = 0;
static const unsigned long CHAR_INTERVAL_MS = 600;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}

    Serial.println("ROCBoard Phase 2 starting...");
    Serial.print("Profile: ");
    Serial.println(PROFILE_14LED.name);
    Serial.print("LED count: ");
    Serial.println(PROFILE_14LED.ledCount);
    Serial.print("LED pin: ");
    Serial.println(PROFILE_14LED.ledPin);

    ledDriver.init(PROFILE_14LED);
    ledDriver.setBrightness(50);
    displayEngine.init(&ledDriver);
    lastCharChange = millis();

    Serial.println("Phase 2 ready — cycling A-Z then 0-9");
}

void loop() {
    unsigned long now = millis();
    if (now - lastCharChange < CHAR_INTERVAL_MS) return;
    lastCharChange = now;

    char c = CYCLE_CHARS[cycleIdx];
    displayEngine.setCharacter(0, c, CRGB::White);
    Serial.printf("Showing: %c\n", c);

    cycleIdx = (cycleIdx + 1) % CYCLE_LEN;
}

#endif // HARDWARE_PROFILE
