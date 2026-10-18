#include "DisplayEngine.h"
#include "LedDriver.h"
#include "LayoutMapper.h"
#include "Font14Segment.h"

#ifdef NATIVE_TEST
// Provide definitions for the CRGB stubs declared in DisplayEngine.h
const CRGB CRGB::Black = CRGB(0, 0, 0);
const CRGB CRGB::White = CRGB(255, 255, 255);
#endif

DisplayEngine::DisplayEngine()
    : _driver(nullptr)
    , _initialized(false)
{}

void DisplayEngine::init(LedDriver* driver) {
    _driver      = driver;
    _initialized = true;
}

void DisplayEngine::setCharacter(uint8_t pos, char c, CRGB color) {
    if (!_initialized || !_driver) return;

    SegmentMask mask = Font14Seg::charToMask(c);

    for (uint8_t seg = 0; seg < 14; seg++) {
        uint16_t physIndex = LayoutMapper::getPhysicalIndex(pos, seg);
        if (physIndex == INVALID_LED_INDEX) continue;

        bool segOn = (mask & (1 << seg)) != 0;
#ifndef NATIVE_TEST
        CRGB pixColor = segOn ? color : CRGB::Black;
        _driver->setPixel(physIndex,
                          pixColor.r,
                          pixColor.g,
                          pixColor.b);
#else
        (void)segOn;
        (void)physIndex;
#endif
    }
    _driver->show();
}

void DisplayEngine::renderText(const char* text, uint8_t startPos) {
    if (!_initialized || !_driver || !text) return;
    uint8_t pos = startPos;
    while (*text) {
#ifndef NATIVE_TEST
        setCharacter(pos, *text, CRGB::White);
#endif
        ++text;
        ++pos;
    }
}

void DisplayEngine::renderChar(char c, uint8_t pos) {
#ifndef NATIVE_TEST
    setCharacter(pos, c, CRGB::White);
#else
    (void)c;
    (void)pos;
#endif
}

void DisplayEngine::clear() {
    if (_driver) {
        _driver->clear();
    }
}
