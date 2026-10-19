#pragma once
// DisplayEngine — maps text/layout to LED pixel data via Font14Segment + LedDriver.
// Phase 2: setCharacter (single char)
// Phase 3: renderText, renderScore, renderState (full scoreboard)

#ifndef NATIVE_TEST
#include <FastLED.h>
#else
// Minimal CRGB stub for native/test builds
struct CRGB {
    uint8_t r, g, b;
    CRGB() : r(0), g(0), b(0) {}
    CRGB(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}
    static const CRGB Black;
    static const CRGB White;
    static CRGB fromRGB(uint32_t packed) {
        return CRGB((packed >> 16) & 0xFF, (packed >> 8) & 0xFF, packed & 0xFF);
    }
};
#endif

class LedDriver;
struct ScoreboardState;

class DisplayEngine {
public:
    DisplayEngine();

    // Initialize with driver pointer. Must be called before any rendering.
    void init(LedDriver* driver);

    // Phase 2: render a single character at position `pos` with `color`.
    // Calls show() after rendering.
    void setCharacter(uint8_t pos, char c, CRGB color);

    // Phase 3: render a null-terminated string starting at characterPosition.
    // Does NOT call show() — caller must call show() or use renderState().
    void renderTextNoShow(const char* text, uint8_t startPos, CRGB color);

    // Phase 3: render 0-99 score at two character positions `pos` and `pos+1`.
    // Does NOT call show().
    void renderScoreNoShow(uint8_t score, uint8_t pos, CRGB color);

    // Phase 3+: render the full scoreboard state — all 24 positions — then show().
    // This is the primary rendering entry point for Phase 3+.
    void renderState(const ScoreboardState& state);

    // Render a null-terminated string starting at characterPosition (white, with show).
    void renderText(const char* text, uint8_t startPos = 0);

    // Render a single character at a position (white, with show).
    void renderChar(char c, uint8_t pos);

    // Run the self-test animation: flash each segment in sequence then all-on then off.
    // Non-blocking: call from loop(); returns true when complete.
    bool runSelfTest();

    // Clear the display (all LEDs off).
    void clear();

private:
    LedDriver* _driver;
    bool       _initialized;

    // Self-test state machine
    uint8_t  _selfTestSeg;
    bool     _selfTestRunning;
    unsigned long _selfTestLastMs;

    // Helper: set one character's segments without show
    void _setCharNoShow(uint8_t pos, char c, CRGB color);
};
