#include "DisplayEngine.h"
// TODO Phase 2: Implement DisplayEngine
// This module maps text content to LED pixel positions using Font14Segment masks
// and forwards pixel data to LedDriver.

DisplayEngine::DisplayEngine()
    : _driver(nullptr)
    , _initialized(false)
{}

void DisplayEngine::init(LedDriver* driver) {
    _driver      = driver;
    _initialized = true;
    // TODO Phase 2: initialize frame buffer, load layout from LayoutMapper
}

void DisplayEngine::renderText(const char* text, uint8_t startPos) {
    (void)text;
    (void)startPos;
    // TODO Phase 2: iterate characters, call Font14Seg::charToMask(), map to LED indices
}

void DisplayEngine::renderChar(char c, uint8_t pos) {
    (void)c;
    (void)pos;
    // TODO Phase 2: single character render path
}

void DisplayEngine::clear() {
    if (_driver) {
        // TODO Phase 2: delegate to LedDriver::clear()
    }
}
