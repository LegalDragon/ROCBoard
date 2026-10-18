#include "LayoutMapper.h"

// LayoutMapper — Phase 2 implementation: single 14-segment character.
// Phase 3 will add full 24-character layout computation.

LayoutMapper::LayoutMapper()
    : _totalLeds(0)
    , _loaded(false)
{}

void LayoutMapper::loadForLedCount(uint16_t totalLeds) {
    _totalLeds = totalLeds;
    _loaded    = true;
    // TODO Phase 3: compute CharLayout table for each of the 24 positions
}

CharLayout LayoutMapper::getCharLayout(uint8_t charPos) const {
    (void)charPos;
    // TODO Phase 3: return computed layout for charPos
    return {0, 14};
}

uint16_t LayoutMapper::getPhysicalIndex(uint8_t charPos, uint8_t segment) {
    // Phase 2 (PROFILE_14_LED): single character at position 0.
    // Segment order: 0=a, 1=b, 2=c, 3=d, 4=e, 5=f, 6=g, 7=h,
    //                8=i, 9=j, 10=k, 11=l, 12=m, 13=dp
    // Each segment maps directly to the LED at the same index.

#if defined(HARDWARE_PROFILE) && (HARDWARE_PROFILE == PROFILE_14_LED)
    if (charPos != 0) return INVALID_LED_INDEX;
    if (segment >= 14) return INVALID_LED_INDEX;
    return static_cast<uint16_t>(segment);
#else
    // Other profiles not yet implemented at the physical-index level.
    // Phase 3 will map charPos * 14 + segment with proper stride.
    (void)charPos;
    (void)segment;
    return INVALID_LED_INDEX;
#endif
}
