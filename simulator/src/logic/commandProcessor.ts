// CommandProcessor — parses JSON command strings and returns updated state.
// Mirrors the firmware CommandProcessor (firmware/src/CommandProcessor.cpp).
// SYNC: keep command set in sync with firmware.

import type { ScoreboardState, RocCommand } from '../types/roc';

export interface CommandResult {
  success: boolean;
  state?: ScoreboardState;
  response?: string;
  error?: string;
}

// Build a 10-char padded name from player name array (same logic as firmware)
function buildTeamName(players: string[]): string {
  const combined = players
    .filter(p => p && p.length > 0)
    .join(' ')
    .toUpperCase()
    .slice(0, 10);
  return combined.padEnd(10, ' ');
}

function cloneState(s: ScoreboardState): ScoreboardState {
  return JSON.parse(JSON.stringify(s)) as ScoreboardState;
}

export function processCommand(json: string, current: ScoreboardState): CommandResult {
  let cmd: RocCommand;
  try {
    cmd = JSON.parse(json) as RocCommand;
  } catch {
    return { success: false, error: 'JSON parse error' };
  }

  const state = cloneState(current);

  // ── ping ──────────────────────────────────────────────────────────────────
  if (cmd.cmd === 'ping' || cmd.cmd === 'PING') {
    return { success: true, state, response: 'PONG' };
  }

  // ── set_display (Phase 4 spec format) ────────────────────────────────────
  if (cmd.cmd === 'set_display') {
    const top    = cmd['top']    as { players: string[]; score: number; color?: string } | undefined;
    const bottom = cmd['bottom'] as { players: string[]; score: number; color?: string } | undefined;

    if (top) {
      state.teamA.name  = buildTeamName(top.players ?? []);
      state.teamA.score = Math.max(0, Math.min(99, top.score ?? state.teamA.score));
      if (top.color) state.teamA.color = top.color;
    }
    if (bottom) {
      state.teamB.name  = buildTeamName(bottom.players ?? []);
      state.teamB.score = Math.max(0, Math.min(99, bottom.score ?? state.teamB.score));
      if (bottom.color) state.teamB.color = bottom.color;
    }
    return { success: true, state, response: 'OK' };
  }

  // ── swap_teams ────────────────────────────────────────────────────────────
  if (cmd.cmd === 'swap_teams') {
    state.swapped = !state.swapped;
    return {
      success: true, state,
      response: `swapped: ${state.swapped}`
    };
  }

  // ── brightness ────────────────────────────────────────────────────────────
  if (cmd.cmd === 'brightness') {
    const val = typeof cmd['value'] === 'number' ? cmd['value'] as number : -1;
    if (val < 0 || val > 255) {
      return { success: false, error: 'value must be 0-255' };
    }
    state.brightness = val;
    return { success: true, state, response: `brightness: ${val}` };
  }

  // ── self_test ─────────────────────────────────────────────────────────────
  if (cmd.cmd === 'self_test') {
    // Simulator: flash the test mode; return success (visual handled by App.tsx)
    return { success: true, state, response: 'self_test: running' };
  }

  // ── clear ─────────────────────────────────────────────────────────────────
  if (cmd.cmd === 'clear' || cmd.cmd === 'CLEAR') {
    state.teamA = { name: '          ', score: 0, color: state.teamA.color };
    state.teamB = { name: '          ', score: 0, color: state.teamB.color };
    return { success: true, state, response: 'OK' };
  }

  // ── SET_SCORE (legacy compat) ─────────────────────────────────────────────
  if (cmd.cmd === 'SET_SCORE') {
    const scoreA = typeof cmd['teamA'] === 'number' ? cmd['teamA'] as number : state.teamA.score;
    const scoreB = typeof cmd['teamB'] === 'number' ? cmd['teamB'] as number : state.teamB.score;
    state.teamA.score = Math.max(0, Math.min(99, scoreA));
    state.teamB.score = Math.max(0, Math.min(99, scoreB));
    return { success: true, state, response: 'OK' };
  }

  // ── SET_NAME (legacy compat) ──────────────────────────────────────────────
  if (cmd.cmd === 'SET_NAME') {
    const team = cmd['team'] as string;
    const name = (cmd['name'] as string ?? '').toUpperCase().slice(0, 10).padEnd(10, ' ');
    if (team === 'A' || team === 'top')    state.teamA.name = name;
    else if (team === 'B' || team === 'bottom') state.teamB.name = name;
    else return { success: false, error: `Unknown team: ${team}` };
    return { success: true, state, response: 'OK' };
  }

  return { success: false, error: `Unknown command: ${cmd.cmd}` };
}
