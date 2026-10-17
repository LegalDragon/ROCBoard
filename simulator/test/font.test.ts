// SHARED: keep in sync with firmware/test/test_font/test_font.cpp
import { describe, it, expect } from 'vitest';
import {
  charToMask,
  SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_H, SEG_DP
} from '../src/logic/font14segment';

describe('Font14Segment', () => {
  it("'A' has segments a, b, c, e, f, g, h lit", () => {
    const mask = charToMask('A');
    expect(mask & SEG_A).toBeTruthy(); // top
    expect(mask & SEG_B).toBeTruthy(); // upper right
    expect(mask & SEG_C).toBeTruthy(); // lower right
    expect(mask & SEG_E).toBeTruthy(); // lower left
    expect(mask & SEG_F).toBeTruthy(); // upper left
    expect(mask & SEG_G).toBeTruthy(); // middle left
    expect(mask & SEG_H).toBeTruthy(); // middle right
    expect(mask & SEG_D).toBeFalsy();  // no bottom
    expect(mask & SEG_DP).toBeFalsy(); // no decimal point
  });

  it("'0' has segments a, b, c, d, e, f lit", () => {
    const mask = charToMask('0');
    expect(mask & SEG_A).toBeTruthy();
    expect(mask & SEG_B).toBeTruthy();
    expect(mask & SEG_C).toBeTruthy();
    expect(mask & SEG_D).toBeTruthy();
    expect(mask & SEG_E).toBeTruthy();
    expect(mask & SEG_F).toBeTruthy();
    // No middle bar
    expect(mask & SEG_G).toBeFalsy();
    expect(mask & SEG_H).toBeFalsy();
    expect(mask & SEG_DP).toBeFalsy();
  });

  it('space returns mask 0 (all segments off)', () => {
    expect(charToMask(' ')).toBe(0);
  });

  it('lowercase is mapped to uppercase', () => {
    expect(charToMask('a')).toBe(charToMask('A'));
    expect(charToMask('z')).toBe(charToMask('Z'));
  });

  it('unrecognized characters return 0', () => {
    expect(charToMask('\x01')).toBe(0);
  });
});
