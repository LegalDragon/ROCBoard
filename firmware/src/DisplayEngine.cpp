#include "DisplayEngine.h"
#include "LedDriver.h"
#include "LayoutMapper.h"
#include "Font14Segment.h"
#include "ScoreboardState.h"

#ifdef NATIVE_TEST
const CRGB CRGB::Black = CRGB(0, 0, 0);
const CRGB CRGB::White = CRGB(255, 255, 255);
#endif

// Dim amber for off-segments: visible indicator of display area, not "black"
static constexpr uint8_t OFF_R = 10, OFF_G = 6, OFF_B = 0;

DisplayEngine::DisplayEngine()
    : _driver(nullptr)
    , _initialized(false)
    , _selfTestSeg(0)
    , _selfTestRunning(false)
    , _selfTestLastMs(0)
{}

void DisplayEngine::init(LedDriver* driver) {
    _driver      = driver;
    _initialized = true;
}

// ─── Internal helpers ──────────────────────────────────────────────────────

void DisplayEngine::_setCharNoShow(uint8_t pos, char c, CRGB color) {
    if (!_initialized || !_driver) return;
    SegmentMask mask = Font14Seg::charToMask(c);
    for (uint8_t seg = 0; seg < 14; seg++) {
        uint16_t physIndex = LayoutMapper::getPhysicalIndex(pos, seg);
        if (physIndex == INVALID_LED_INDEX) continue;
        bool segOn = (mask & (1 << seg)) != 0;
#ifndef NATIVE_TEST
        if (segOn) {
            _driver->setPixel(physIndex, color.r, color.g, color.b);
        } else {
            _driver->setPixel(physIndex, OFF_R, OFF_G, OFF_B);
        }
#else
        (void)segOn;
#endif
    }
}

// ─── Phase 2 API ───────────────────────────────────────────────────────────

void DisplayEngine::setCharacter(uint8_t pos, char c, CRGB color) {
    _setCharNoShow(pos, c, color);
    if (_driver) _driver->show();
}

// ─── Phase 3 API ───────────────────────────────────────────────────────────

void DisplayEngine::renderTextNoShow(const char* text, uint8_t startPos, CRGB color) {
    if (!_initialized || !_driver || !text) return;
    uint8_t pos = startPos;
    while (*text) {
        _setCharNoShow(pos, *text, color);
        ++text;
        ++pos;
    }
}

void DisplayEngine::renderScoreNoShow(uint8_t score, uint8_t pos, CRGB color) {
    if (score > 99) score = 99;
    _setCharNoShow(pos,     '0' + (score / 10), color);
    _setCharNoShow(pos + 1, '0' + (score % 10), color);
}

void DisplayEngine::renderState(const ScoreboardState& state) {
    if (!_initialized || !_driver) return;

    // Convert packed colors
#ifndef NATIVE_TEST
    CRGB topC  = CRGB((state.topColor    >> 16) & 0xFF,
                      (state.topColor    >>  8) & 0xFF,
                       state.topColor           & 0xFF);
    CRGB botC  = CRGB((state.bottomColor >> 16) & 0xFF,
                      (state.bottomColor >>  8) & 0xFF,
                       state.bottomColor         & 0xFF);
#else
    CRGB topC  = CRGB::White;
    CRGB botC  = CRGB::White;
#endif

    // Build padded names (10 chars each)
    char nameTop[11], nameBot[11];
    snprintf(nameTop, sizeof(nameTop), "%-10.10s",
             state.swapped ? state.bottomName : state.topName);
    snprintf(nameBot, sizeof(nameBot), "%-10.10s",
             state.swapped ? state.topName : state.bottomName);

    uint8_t scoreTop = state.swapped ? state.bottomScore : state.topScore;
    uint8_t scoreBot = state.swapped ? state.topScore    : state.bottomScore;
    CRGB    colorTop = state.swapped ? botC : topC;
    CRGB    colorBot = state.swapped ? topC : botC;

    renderTextNoShow(nameTop,  0, colorTop);
    renderTextNoShow(nameBot, 10, colorBot);
    renderScoreNoShow(scoreTop, 20, colorTop);
    renderScoreNoShow(scoreBot, 22, colorBot);

#ifndef NATIVE_TEST
    _driver->show();
#endif
}

// ─── Legacy / compat API ──────────────────────────────────────────────────

void DisplayEngine::renderText(const char* text, uint8_t startPos) {
    renderTextNoShow(text, startPos, CRGB::White);
    if (_driver) _driver->show();
}

void DisplayEngine::renderChar(char c, uint8_t pos) {
    setCharacter(pos, c, CRGB::White);
}

// ─── Self-test ─────────────────────────────────────────────────────────────

bool DisplayEngine::runSelfTest() {
    if (!_initialized || !_driver) return true; // can't run, report done

#ifndef NATIVE_TEST
    unsigned long now = millis();
    const unsigned long STEP_MS = 80;

    if (!_selfTestRunning) {
        _selfTestRunning = true;
        _selfTestSeg     = 0;
        _selfTestLastMs  = now;
    }

    if (now - _selfTestLastMs < STEP_MS) return false; // still waiting
    _selfTestLastMs = now;

    // Steps 0-13: light one segment at a time across all char positions
    if (_selfTestSeg < 14) {
        _driver->clear();
        // Light segment _selfTestSeg for all character positions
        for (uint8_t pos = 0; pos < 24; pos++) {
            uint16_t idx = LayoutMapper::getPhysicalIndex(pos, _selfTestSeg);
            if (idx != INVALID_LED_INDEX) {
                _driver->setPixel(idx, 0, 180, 255);
            }
        }
        _driver->show();
        _selfTestSeg++;
        return false;
    }

    // Step 14: all segments on (white)
    if (_selfTestSeg == 14) {
        for (uint16_t i = 0; i < _driver->ledCount(); i++) {
            _driver->setPixel(i, 200, 200, 200);
        }
        _driver->show();
        _selfTestSeg++;
        return false;
    }

    // Step 15: all off — done
    _driver->clear();
    _selfTestRunning = false;
    _selfTestSeg     = 0;
    return true;
#else
    return true; // native test: instant complete
#endif
}

// ─── Clear ─────────────────────────────────────────────────────────────────

void DisplayEngine::clear() {
    if (_driver) _driver->clear();
}
