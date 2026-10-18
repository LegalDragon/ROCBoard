#pragma once
// DisplayEngine — maps text/layout to LED pixel data via Font14Segment + LedDriver.
// Phase 2+ implementation.

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
};
#endif

class LedDriver;

class DisplayEngine {
public:
    DisplayEngine();

    // Initialize with driver reference and layout configuration.
    void init(LedDriver* driver);

    // Phase 2: render a single character at a position using the given color.
    // Looks up the 14-segment bitmask, then calls LayoutMapper::getPhysicalIndex
    // for each segment, and sets each LED on/off via LedDriver.
    void setCharacter(uint8_t pos, char c, CRGB color);

    // Render a null-terminated string starting at characterPosition.
    // TODO Phase 2: implement full layout rendering
    void renderText(const char* text, uint8_t startPos = 0);

    // Render a single character at a position (alias for setCharacter with white).
    void renderChar(char c, uint8_t pos);

    // Clear the display.
    void clear();

private:
    LedDriver* _driver;
    bool       _initialized;
};
