#include "LedDriver.h"

#ifndef NATIVE_TEST
#include <FastLED.h>

static CRGB* _leds = nullptr;
#endif

LedDriver::LedDriver()
    : _initialized(false)
    , _ledCount(0)
{}

void LedDriver::init(const HardwareProfile& profile) {
    _ledCount    = profile.ledCount;
    _initialized = true;

#ifndef NATIVE_TEST
    if (_leds) {
        delete[] _leds;
    }
    _leds = new CRGB[_ledCount];

    // FastLED dynamic setup — pin must be a compile-time constant for the
    // template, so we branch on the known pins in our profiles.
    // Add cases here when new hardware profiles are introduced.
    switch (profile.ledPin) {
        case 48:
            FastLED.addLeds<WS2812B, 48, GRB>(_leds, _ledCount);
            break;
        default:
            // Fallback: attempt GPIO48 (onboard LED on DevKitC-1 N8R8)
            FastLED.addLeds<WS2812B, 48, GRB>(_leds, _ledCount);
            break;
    }
    FastLED.setBrightness(50);
    clear();
#endif
}

void LedDriver::setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!_initialized) return;
    if (index >= _ledCount) index = _ledCount - 1;

#ifndef NATIVE_TEST
    _leds[index] = CRGB(r, g, b);
#endif
}

void LedDriver::show() {
    if (!_initialized) return;
#ifndef NATIVE_TEST
    FastLED.show();
#endif
}

void LedDriver::clear() {
    if (!_initialized) return;
#ifndef NATIVE_TEST
    for (uint16_t i = 0; i < _ledCount; i++) {
        _leds[i] = CRGB::Black;
    }
    FastLED.show();
#endif
}

uint16_t LedDriver::ledCount() const {
    return _ledCount;
}
