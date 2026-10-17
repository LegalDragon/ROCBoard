// SHARED: keep in sync with firmware/src/Font14Segment.h
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
//  Bit  7 = h (upper-middle horizontal, right half)
//  Bit  8 = i (upper-center vertical)
//  Bit  9 = j (diagonal: top-left to center)
//  Bit 10 = k (diagonal: center to top-right)
//  Bit 11 = l (lower-center vertical)
//  Bit 12 = m (diagonal: center to bottom-right)
//  Bit 13 = n (decimal point)

export const SEG_A  = 1 << 0;
export const SEG_B  = 1 << 1;
export const SEG_C  = 1 << 2;
export const SEG_D  = 1 << 3;
export const SEG_E  = 1 << 4;
export const SEG_F  = 1 << 5;
export const SEG_G  = 1 << 6;
export const SEG_H  = 1 << 7;
export const SEG_I  = 1 << 8;
export const SEG_J  = 1 << 9;
export const SEG_K  = 1 << 10;
export const SEG_L  = 1 << 11;
export const SEG_M  = 1 << 12;
export const SEG_DP = 1 << 13;

// Segment names for SVG rendering — must match bit order above
export const SEGMENT_NAMES = ['a','b','c','d','e','f','g','h','i','j','k','l','m','dp'] as const;
export type SegmentName = typeof SEGMENT_NAMES[number];

// ASCII character table (indices 0-127)
// Entries use the same bit patterns as firmware/src/Font14Segment.cpp
const CHAR_TABLE: number[] = new Array(128).fill(0);

// Digits 0-9
CHAR_TABLE[0x30] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;           // 0
CHAR_TABLE[0x31] = SEG_B | SEG_C;                                             // 1
CHAR_TABLE[0x32] = SEG_A | SEG_B | SEG_G | SEG_H | SEG_D | SEG_E;           // 2
CHAR_TABLE[0x33] = SEG_A | SEG_B | SEG_G | SEG_H | SEG_C | SEG_D;           // 3
CHAR_TABLE[0x34] = SEG_F | SEG_G | SEG_H | SEG_B | SEG_C;                   // 4
CHAR_TABLE[0x35] = SEG_A | SEG_F | SEG_G | SEG_H | SEG_C | SEG_D;           // 5
CHAR_TABLE[0x36] = SEG_A | SEG_F | SEG_G | SEG_H | SEG_C | SEG_D | SEG_E;  // 6
CHAR_TABLE[0x37] = SEG_A | SEG_B | SEG_C;                                    // 7
CHAR_TABLE[0x38] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_H; // 8
CHAR_TABLE[0x39] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G | SEG_H;  // 9

// Uppercase A-Z
CHAR_TABLE[0x41] = SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G | SEG_H;   // A
CHAR_TABLE[0x42] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_H | SEG_I | SEG_L;   // B
CHAR_TABLE[0x43] = SEG_A | SEG_D | SEG_E | SEG_F;                            // C
CHAR_TABLE[0x44] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_I | SEG_L;           // D
CHAR_TABLE[0x45] = SEG_A | SEG_D | SEG_E | SEG_F | SEG_G | SEG_H;           // E
CHAR_TABLE[0x46] = SEG_A | SEG_E | SEG_F | SEG_G | SEG_H;                   // F
CHAR_TABLE[0x47] = SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_H;           // G
CHAR_TABLE[0x48] = SEG_B | SEG_C | SEG_E | SEG_F | SEG_G | SEG_H;           // H
CHAR_TABLE[0x49] = SEG_A | SEG_D | SEG_I | SEG_L;                            // I
CHAR_TABLE[0x4A] = SEG_B | SEG_C | SEG_D | SEG_E;                            // J
CHAR_TABLE[0x4B] = SEG_E | SEG_F | SEG_G | SEG_J | SEG_M;                   // K
CHAR_TABLE[0x4C] = SEG_D | SEG_E | SEG_F;                                    // L
CHAR_TABLE[0x4D] = SEG_B | SEG_C | SEG_E | SEG_F | SEG_J | SEG_K;           // M
CHAR_TABLE[0x4E] = SEG_B | SEG_C | SEG_E | SEG_F | SEG_J | SEG_M;           // N
CHAR_TABLE[0x4F] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;           // O
CHAR_TABLE[0x50] = SEG_A | SEG_B | SEG_E | SEG_F | SEG_G | SEG_H;           // P
CHAR_TABLE[0x51] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_M;  // Q
CHAR_TABLE[0x52] = SEG_A | SEG_B | SEG_E | SEG_F | SEG_G | SEG_H | SEG_M;  // R
CHAR_TABLE[0x53] = SEG_A | SEG_C | SEG_D | SEG_F | SEG_G | SEG_H;           // S
CHAR_TABLE[0x54] = SEG_A | SEG_I | SEG_L;                                    // T
CHAR_TABLE[0x55] = SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;                   // U
CHAR_TABLE[0x56] = SEG_E | SEG_F | SEG_K | SEG_L;                           // V
CHAR_TABLE[0x57] = SEG_B | SEG_C | SEG_E | SEG_F | SEG_L | SEG_M;          // W
CHAR_TABLE[0x58] = SEG_J | SEG_K | SEG_L | SEG_M;                           // X
CHAR_TABLE[0x59] = SEG_F | SEG_J | SEG_K | SEG_C;                           // Y
CHAR_TABLE[0x5A] = SEG_A | SEG_K | SEG_D | SEG_E;                           // Z

// Space
CHAR_TABLE[0x20] = 0;

// Copy uppercase to lowercase
for (let i = 0x61; i <= 0x7A; i++) {
  CHAR_TABLE[i] = CHAR_TABLE[i - 0x20];
}

export function charToMask(c: string): number {
  const code = c.toUpperCase().charCodeAt(0);
  if (code < 0 || code >= 128) return 0;
  return CHAR_TABLE[code];
}

export function segmentIsOn(mask: number, segIndex: number): boolean {
  return (mask & (1 << segIndex)) !== 0;
}
