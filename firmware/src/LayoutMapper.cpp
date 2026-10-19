#include "LayoutMapper.h"

// LayoutMapper — maps logical (charPos, segment) → physical LED index.
//
// Physical layout: all characters use 14 segments.
// LED index formula: charPos * 14 + segment
//
// Full board positions (24 chars × 14 segs = 336 LEDs):
//   Pos  0– 9: Top team alpha name    (LEDs   0–139)
//   Pos 10–19: Bottom team alpha name (LEDs 140–279)
//   Pos 20–21: Top team score         (LEDs 280–307)
//   Pos 22–23: Bottom team score      (LEDs 308–335)
//
// Segment order (0-13):
//   0=a(top), 1=b(upper-right), 2=c(lower-right), 3=d(bottom),
//   4=e(lower-left), 5=f(upper-left), 6=g(middle-left), 7=h(middle-right),
//   8=i(center-vertical-top), 9=j(diag TL→C), 10=k(diag TR→C),
//   11=l(center-vertical-bottom), 12=m(diag C→BR), 13=dp(decimal)

LayoutMapper::LayoutMapper()
    : _totalLeds(0)
    , _loaded(false)
{}

void LayoutMapper::loadForLedCount(uint16_t totalLeds) {
    _totalLeds = totalLeds;
    _loaded    = true;
}

CharLayout LayoutMapper::getCharLayout(uint8_t charPos) const {
    return { static_cast<uint16_t>(charPos * 14), 14 };
}

uint16_t LayoutMapper::getPhysicalIndex(uint8_t charPos, uint8_t segment) {
    if (segment >= 14) return INVALID_LED_INDEX;

#if defined(HARDWARE_PROFILE) && (HARDWARE_PROFILE == PROFILE_14_LED)
    // Phase 2: single character at position 0 only
    if (charPos != 0) return INVALID_LED_INDEX;
    return static_cast<uint16_t>(segment);

#elif defined(HARDWARE_PROFILE) && (HARDWARE_PROFILE == PROFILE_28_LED)
    // Phase 3a: two characters at positions 0-1
    if (charPos >= 2) return INVALID_LED_INDEX;
    return static_cast<uint16_t>(charPos * 14 + segment);

#elif defined(HARDWARE_PROFILE) && (HARDWARE_PROFILE == PROFILE_FULL_ID)
    // Phase 3+: full 24-character scoreboard (positions 0-23)
    if (charPos >= 24) return INVALID_LED_INDEX;
    return static_cast<uint16_t>(charPos * 14 + segment);

#else
    // Default (including native test builds): full mapping
    if (charPos >= 24) return INVALID_LED_INDEX;
    return static_cast<uint16_t>(charPos * 14 + segment);
#endif
}
