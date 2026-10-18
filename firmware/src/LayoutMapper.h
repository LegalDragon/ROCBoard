#pragma once
#include <stdint.h>
#include "../include/HardwareProfiles.h"

// LayoutMapper — maps logical character positions to physical LED index ranges.
// Phase 2: single 14-segment character (PROFILE_14_LED).
// Phase 3+: full 24-character layout.
//
// Full scoreboard layout (24 characters total):
//   Positions 0-9:   Team A name (10 × 14-segment alpha chars)
//   Positions 10-19: Team B name (10 × 14-segment alpha chars)
//   Positions 20-21: Score A (2 × numeric chars)
//   Positions 22-23: Score B (2 × numeric chars)

static constexpr uint16_t INVALID_LED_INDEX = 0xFFFF;

struct CharLayout {
    uint16_t firstLedIndex; // First LED in this character's segment group
    uint8_t  segmentCount;  // 14 for alpha, 7 for numeric
};

class LayoutMapper {
public:
    LayoutMapper();

    // Load layout for a given total LED count.
    // TODO Phase 3: implement layout computation
    void loadForLedCount(uint16_t totalLeds);

    // Get layout for a character position (0-23).
    // TODO Phase 3: return computed CharLayout
    CharLayout getCharLayout(uint8_t charPos) const;

    // Phase 2: get physical LED index for a given character position and segment (0-13).
    // For PROFILE_14_LED: charPos must be 0, segment maps directly to LED index.
    // Returns INVALID_LED_INDEX (0xFFFF) if charPos or segment is out of range.
    static uint16_t getPhysicalIndex(uint8_t charPos, uint8_t segment);

private:
    uint16_t _totalLeds;
    bool     _loaded;
    // TODO Phase 3: store computed layout table
};
