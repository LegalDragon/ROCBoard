#pragma once
// SHARED: keep in sync with simulator/src/logic/font14segment.ts
//
// 14-Segment display bit layout:
//
//      _aaa_
//     |     |
//    f|  g  |b
//     |_____|
//    e| h i |c
//     |     |
//     |_____|
//       ddd
// + j k l m (diagonal segments) + n (decimal point)
//
// Bit positions:
//  Bit  0 = a (top horizontal)
//  Bit  1 = b (upper-right vertical)
//  Bit  2 = c (lower-right vertical)
//  Bit  3 = d (bottom horizontal)
//  Bit  4 = e (lower-left vertical)
//  Bit  5 = f (upper-left vertical)
//  Bit  6 = g (upper-middle horizontal, left half)
//  Bit  7 = h (upper-middle horizontal, right half — some displays merge g+h)
//  Bit  8 = i (upper-center vertical)
//  Bit  9 = j (diagonal: top-left to center)
//  Bit 10 = k (diagonal: center to top-right)
//  Bit 11 = l (lower-center vertical)
//  Bit 12 = m (diagonal: center to bottom-right)
//  Bit 13 = n (decimal point)

#include <stdint.h>

// 16-bit mask (bits 0-13 used)
using SegmentMask = uint16_t;

namespace Font14Seg {
    // Segment bit constants
    static constexpr SegmentMask SEG_A  = (1 << 0);
    static constexpr SegmentMask SEG_B  = (1 << 1);
    static constexpr SegmentMask SEG_C  = (1 << 2);
    static constexpr SegmentMask SEG_D  = (1 << 3);
    static constexpr SegmentMask SEG_E  = (1 << 4);
    static constexpr SegmentMask SEG_F  = (1 << 5);
    static constexpr SegmentMask SEG_G  = (1 << 6);
    static constexpr SegmentMask SEG_H  = (1 << 7);
    static constexpr SegmentMask SEG_I  = (1 << 8);
    static constexpr SegmentMask SEG_J  = (1 << 9);
    static constexpr SegmentMask SEG_K  = (1 << 10);
    static constexpr SegmentMask SEG_L  = (1 << 11);
    static constexpr SegmentMask SEG_M  = (1 << 12);
    static constexpr SegmentMask SEG_DP = (1 << 13);

    // Returns the segment mask for a character (uppercase A-Z, 0-9, space).
    // Returns 0 for unrecognized characters (shows as blank).
    SegmentMask charToMask(char c);

    // Lookup table — 128 entries (ASCII).
    extern const SegmentMask CHAR_TABLE[128];
}
