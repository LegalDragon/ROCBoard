// ROCBoard firmware — multi-profile entry point.
// Profile is selected at compile time via HARDWARE_PROFILE build flag.
// Phase 1: single onboard WS2812B blink (GPIO48, PROFILE_1_LED).
// Phase 2: 14-segment single character cycle A→Z→0→9 (GPIO1, PROFILE_14_LED).
// Phase 3/4/5: Full scoreboard with JSON serial command protocol (PROFILE_FULL_ID).
// Uses millis() for non-blocking timing. NO delay() calls (except Diagnostics POST).

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

static const char CYCLE_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static const uint8_t CYCLE_LEN  = sizeof(CYCLE_CHARS) - 1;

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

// ─────────────────────────────────────────────
// PHASE 3 / 4 / 5: Full scoreboard + JSON serial
// ─────────────────────────────────────────────
#elif HARDWARE_PROFILE == PROFILE_FULL_ID

#include "ScoreboardState.h"
#include "CommandProcessor.h"
#include "TransportManager.h"
#include "Diagnostics.h"

static LedDriver         ledDriver;
static DisplayEngine     displayEngine;
static ScoreboardState   boardState;
static CommandProcessor  cmdProcessor;
static TransportManager  transport;
static Diagnostics       diagnostics;

// Track whether a re-render is needed
static volatile bool     _stateChanged = false;
// Track self-test request
static volatile bool     _selfTestRequested = false;
static bool              _selfTestRunning   = false;

// Callback: fires when CommandProcessor updates boardState
static void onStateChanged() {
    _stateChanged = true;
}

// Callback: fires when self_test command received
static void onSelfTest() {
    _selfTestRequested = true;
}

// Apply brightness and re-render from current state
static void applyStateToDisplay() {
    ledDriver.setBrightness(boardState.brightness);
    displayEngine.renderState(boardState);
    _stateChanged = false;
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}

    Serial.println("ROCBoard Phase 3/4/5 starting...");
    Serial.print("Profile: "); Serial.println(PROFILE_FULL.name);
    Serial.print("LEDs: ");    Serial.println(PROFILE_FULL.ledCount);

    // Initialize subsystems
    ledDriver.init(PROFILE_FULL);
    ledDriver.setBrightness(boardState.brightness);
    displayEngine.init(&ledDriver);

    // POST
    diagnostics.log("INFO", "Running POST...");
    diagnostics.runPost(&ledDriver);

    // Wire up command processing
    cmdProcessor.init(&displayEngine, &boardState);
    cmdProcessor.setOnStateChanged(onStateChanged);
    cmdProcessor.setOnSelfTest(onSelfTest);

    transport.init(&cmdProcessor);
    transport.enableSerial(true);

    // Render initial state (blank display, ready)
    applyStateToDisplay();

    diagnostics.log("INFO", "Ready. Send JSON commands over serial at 115200 baud.");
    diagnostics.log("INFO", "Example: {\"cmd\":\"set_display\",\"top\":{\"players\":[\"JOHN\",\"MIKE\"],\"score\":0,\"color\":\"#FF0000\"},\"bottom\":{\"players\":[\"DAVID\",\"PETER\"],\"score\":0,\"color\":\"#0000FF\"}}");
}

void loop() {
    // Poll serial for incoming commands
    transport.poll();

    // Handle self-test request
    if (_selfTestRequested && !_selfTestRunning) {
        _selfTestRequested = false;
        _selfTestRunning   = true;
    }

    if (_selfTestRunning) {
        bool done = displayEngine.runSelfTest();
        if (done) {
            _selfTestRunning = false;
            _stateChanged    = true; // re-render scoreboard after test
        }
        return; // Don't re-render while self-test is in progress
    }

    // Re-render if state changed
    if (_stateChanged) {
        applyStateToDisplay();
    }
}

#endif // HARDWARE_PROFILE
