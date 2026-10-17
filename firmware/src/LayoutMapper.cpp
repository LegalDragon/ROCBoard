#include "LayoutMapper.h"
// TODO Phase 3: Implement LayoutMapper
// This module computes the physical LED index for each segment of each character
// based on the total LED count and wiring order.

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
