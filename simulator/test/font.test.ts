// SYNC: these tests mirror firmware/test/test_font/test_font.cpp — keep in sync
import { describe, it, expect } from 'vitest';
import {
  charToMask,
  SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_H, SEG_DP
} from '../src/logic/font14segment';
import { processCommand } from '../src/logic/commandProcessor';
import type { ScoreboardState } from '../src/types/roc';

// ─── Font14Segment tests ──────────────────────────────────────────────────

describe('Font14Segment', () => {
  it("'A' has segments a, b, c, e, f, g, h lit", () => {
    const mask = charToMask('A');
    expect(mask & SEG_A).toBeTruthy();
    expect(mask & SEG_B).toBeTruthy();
    expect(mask & SEG_C).toBeTruthy();
    expect(mask & SEG_E).toBeTruthy();
    expect(mask & SEG_F).toBeTruthy();
    expect(mask & SEG_G).toBeTruthy();
    expect(mask & SEG_H).toBeTruthy();
    expect(mask & SEG_D).toBeFalsy();
    expect(mask & SEG_DP).toBeFalsy();
  });

  it("'0' has segments a, b, c, d, e, f lit", () => {
    const mask = charToMask('0');
    expect(mask & SEG_A).toBeTruthy();
    expect(mask & SEG_B).toBeTruthy();
    expect(mask & SEG_C).toBeTruthy();
    expect(mask & SEG_D).toBeTruthy();
    expect(mask & SEG_E).toBeTruthy();
    expect(mask & SEG_F).toBeTruthy();
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

  it('all A-Z chars return non-zero masks', () => {
    for (let i = 0; i < 26; i++) {
      const c = String.fromCharCode(65 + i);
      expect(charToMask(c)).toBeGreaterThan(0);
    }
  });

  it('all 0-9 digits return non-zero masks', () => {
    for (let i = 0; i <= 9; i++) {
      const c = String(i);
      expect(charToMask(c)).toBeGreaterThan(0);
    }
  });
});

// ─── CommandProcessor tests ───────────────────────────────────────────────

const BASE_STATE: ScoreboardState = {
  teamA: { name: 'INITIAL   ', score: 0, color: '#FF0000' },
  teamB: { name: 'INITIAL   ', score: 0, color: '#0000FF' },
  brightness: 50,
  swapped: false,
};

describe('CommandProcessor', () => {
  it('ping returns PONG', () => {
    const result = processCommand('{"cmd":"ping"}', BASE_STATE);
    expect(result.success).toBe(true);
    expect(result.response).toBe('PONG');
  });

  it('set_display updates top and bottom teams', () => {
    const result = processCommand(
      JSON.stringify({
        cmd: 'set_display',
        top:    { players: ['JOHN', 'MIKE'],   score: 8, color: '#FF4400' },
        bottom: { players: ['DAVID', 'PETER'], score: 6, color: '#0066FF' },
      }),
      BASE_STATE
    );
    expect(result.success).toBe(true);
    expect(result.state?.teamA.score).toBe(8);
    expect(result.state?.teamB.score).toBe(6);
    expect(result.state?.teamA.name).toContain('JOHN');
    expect(result.state?.teamA.color).toBe('#FF4400');
    expect(result.state?.teamB.color).toBe('#0066FF');
  });

  it('set_display single player name', () => {
    const result = processCommand(
      JSON.stringify({ cmd: 'set_display', top: { players: ['SYNTHIA'], score: 11 } }),
      BASE_STATE
    );
    expect(result.success).toBe(true);
    expect(result.state?.teamA.name.trim()).toBe('SYNTHIA');
  });

  it('swap_teams toggles swapped flag', () => {
    const r1 = processCommand('{"cmd":"swap_teams"}', BASE_STATE);
    expect(r1.state?.swapped).toBe(true);
    const r2 = processCommand('{"cmd":"swap_teams"}', r1.state!);
    expect(r2.state?.swapped).toBe(false);
  });

  it('brightness updates brightness value', () => {
    const result = processCommand('{"cmd":"brightness","value":200}', BASE_STATE);
    expect(result.success).toBe(true);
    expect(result.state?.brightness).toBe(200);
  });

  it('brightness rejects out-of-range value', () => {
    const result = processCommand('{"cmd":"brightness","value":300}', BASE_STATE);
    expect(result.success).toBe(false);
    expect(result.state).toBeUndefined();
  });

  it('clear resets scores to 0', () => {
    const state = { ...BASE_STATE, teamA: { ...BASE_STATE.teamA, score: 9 } };
    const result = processCommand('{"cmd":"clear"}', state);
    expect(result.state?.teamA.score).toBe(0);
    expect(result.state?.teamB.score).toBe(0);
  });

  it('SET_SCORE updates scores (legacy)', () => {
    const result = processCommand('{"cmd":"SET_SCORE","teamA":11,"teamB":8}', BASE_STATE);
    expect(result.state?.teamA.score).toBe(11);
    expect(result.state?.teamB.score).toBe(8);
  });

  it('unknown command returns error', () => {
    const result = processCommand('{"cmd":"nonexistent"}', BASE_STATE);
    expect(result.success).toBe(false);
    expect(result.error).toBeTruthy();
  });

  it('invalid JSON returns error', () => {
    const result = processCommand('not json', BASE_STATE);
    expect(result.success).toBe(false);
  });
});
