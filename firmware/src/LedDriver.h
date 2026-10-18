#pragma once
#include "../include/HardwareProfiles.h"

// LedDriver — thin wrapper around FastLED
// All methods are non-blocking (no delay() calls).

class LedDriver {
public:
    LedDriver();

    // Initialize with given hardware profile.
    // Must be called before any other method.
    void init(const HardwareProfile& profile);

    // Set a single pixel by index (0-based). Clamps to ledCount-1.
    void setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

    // Push the current framebuffer to the LED strip.
    void show();

    // Set all pixels to black and push.
    void clear();

    // Set global brightness (0-255).
    void setBrightness(uint8_t b);

    // Returns the number of LEDs in the current profile.
    uint16_t ledCount() const;

private:
    bool     _initialized;
    uint16_t _ledCount;
};
