#pragma once
#include <stdint.h>
// LayoutMapper — maps logical character positions to physical LED index ranges.
// Phase 3+ implementation.
//
// Full scoreboard layout (24 characters total):
//   Positions 0-9:   Team A name (10 × 14-segment alpha chars)
//   Positions 10-19: Team B name (10 × 14-segment alpha chars)
//   Positions 20-21: Score A (2 × numeric chars)
//   Positions 22-23: Score B (2 × numeric chars)

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

private:
    uint16_t _totalLeds;
    bool     _loaded;
    // TODO Phase 3: store computed layout table
};
