#pragma once
// DisplayEngine — maps text/layout to LED pixel data via Font14Segment + LedDriver.
// Phase 2+ implementation.

class LedDriver;

class DisplayEngine {
public:
    DisplayEngine();

    // Initialize with driver reference and layout configuration.
    void init(LedDriver* driver);

    // Render a null-terminated string starting at characterPosition.
    // TODO Phase 2: implement full layout rendering
    void renderText(const char* text, uint8_t startPos = 0);

    // Render a single character at a position.
    // TODO Phase 2: implement character rendering
    void renderChar(char c, uint8_t pos);

    // Clear the display.
    void clear();

private:
    LedDriver* _driver;
    bool       _initialized;
    // TODO Phase 2: add frame buffer and layout state
};
