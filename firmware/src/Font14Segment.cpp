#include "Font14Segment.h"
// SHARED: keep in sync with simulator/src/logic/font14segment.ts

namespace Font14Seg {

// Segment masks for ASCII characters 0x00-0x7F.
// Only printable chars are defined; others are 0 (blank).
// All alphabetic entries are for uppercase; lowercase is mapped to uppercase in charToMask().
const SegmentMask CHAR_TABLE[128] = {
    // 0x00-0x1F: control chars — all blank
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

    // 0x20 ' ' (space)
    0,
    // 0x21 '!' — right verticals
    SEG_B | SEG_C,
    // 0x22 '"' — upper right + decimal (treating as two top strokes)
    SEG_B | SEG_F,
    // 0x23-0x2F — minimal punctuation
    0, 0, 0, 0,
    // 0x27 "'" — upper-left vertical
    SEG_F,
    0, 0, 0, 0, 0, 0, 0, 0,

    // 0x30 '0' — a b c d e f (no middle)
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
    // 0x31 '1' — b c
    SEG_B | SEG_C,
    // 0x32 '2' — a b g h d e
    SEG_A | SEG_B | SEG_G | SEG_H | SEG_D | SEG_E,
    // 0x33 '3' — a b g h c d
    SEG_A | SEG_B | SEG_G | SEG_H | SEG_C | SEG_D,
    // 0x34 '4' — f g h b c
    SEG_F | SEG_G | SEG_H | SEG_B | SEG_C,
    // 0x35 '5' — a f g h c d
    SEG_A | SEG_F | SEG_G | SEG_H | SEG_C | SEG_D,
    // 0x36 '6' — a f g h c d e
    SEG_A | SEG_F | SEG_G | SEG_H | SEG_C | SEG_D | SEG_E,
    // 0x37 '7' — a b c
    SEG_A | SEG_B | SEG_C,
    // 0x38 '8' — all segments except diagonals + dp
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_H,
    // 0x39 '9' — a b c d f g h
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G | SEG_H,

    // 0x3A-0x3F punctuation
    0, 0, 0, 0, 0,
    // '?' — a b g h e
    SEG_A | SEG_B | SEG_G | SEG_H | SEG_E,

    // 0x40 '@'
    0,

    // 0x41 'A' — a b c e f g h
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G | SEG_H,
    // 0x42 'B' — a b c d g h i l (using center vertical + right)
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_H | SEG_I | SEG_L,
    // 0x43 'C' — a d e f
    SEG_A | SEG_D | SEG_E | SEG_F,
    // 0x44 'D' — a b c d i l
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_I | SEG_L,
    // 0x45 'E' — a d e f g h
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G | SEG_H,
    // 0x46 'F' — a e f g h
    SEG_A | SEG_E | SEG_F | SEG_G | SEG_H,
    // 0x47 'G' — a c d e f h
    SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_H,
    // 0x48 'H' — b c e f g h
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_G | SEG_H,
    // 0x49 'I' — a d i l
    SEG_A | SEG_D | SEG_I | SEG_L,
    // 0x4A 'J' — b c d e
    SEG_B | SEG_C | SEG_D | SEG_E,
    // 0x4B 'K' — e f g j m
    SEG_E | SEG_F | SEG_G | SEG_J | SEG_M,
    // 0x4C 'L' — d e f
    SEG_D | SEG_E | SEG_F,
    // 0x4D 'M' — b c e f i j k  (two diagonals up from base)
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_J | SEG_K,
    // 0x4E 'N' — b c e f j m
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_J | SEG_M,
    // 0x4F 'O' — a b c d e f
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
    // 0x50 'P' — a b e f g h
    SEG_A | SEG_B | SEG_E | SEG_F | SEG_G | SEG_H,
    // 0x51 'Q' — a b c d e f m
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_M,
    // 0x52 'R' — a b e f g h m
    SEG_A | SEG_B | SEG_E | SEG_F | SEG_G | SEG_H | SEG_M,
    // 0x53 'S' — a c d f g h
    SEG_A | SEG_C | SEG_D | SEG_F | SEG_G | SEG_H,
    // 0x54 'T' — a i l
    SEG_A | SEG_I | SEG_L,
    // 0x55 'U' — b c d e f
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
    // 0x56 'V' — e f k l  (down-pointing chevron)
    SEG_E | SEG_F | SEG_K | SEG_L,
    // 0x57 'W' — b c e f l m  (two diagonals down from top)
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_L | SEG_M,
    // 0x58 'X' — j k m l  (diagonal cross)
    SEG_J | SEG_K | SEG_L | SEG_M,
    // 0x59 'Y' — f j k c (top half diagonal, lower right)
    SEG_F | SEG_J | SEG_K | SEG_C,
    // 0x5A 'Z' — a k d e  (Z shape with diagonal)
    SEG_A | SEG_K | SEG_D | SEG_E,

    // 0x5B-0x60 misc punctuation
    0,0,0,0,0,0,

    // 0x61-0x7A lowercase a-z — same as uppercase (handled in charToMask)
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

    // 0x7B-0x7F
    0,0,0,0,0
};

SegmentMask charToMask(char c) {
    // Normalize to uppercase
    if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    }
    uint8_t idx = static_cast<uint8_t>(c);
    if (idx >= 128) return 0;
    return CHAR_TABLE[idx];
}

} // namespace Font14Seg
