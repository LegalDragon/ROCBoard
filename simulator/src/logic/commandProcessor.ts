// CommandProcessor — parses JSON command strings and returns updated state.
// Phase 4+ will wire this to the CommandConsole and WebSocket transport.

import type { ScoreboardState, RocCommand } from '../types/roc';

export interface CommandResult {
  success: boolean;
  state?: ScoreboardState;
  response?: string;
  error?: string;
}

export function processCommand(json: string, current: ScoreboardState): CommandResult {
  let cmd: RocCommand;
  try {
    cmd = JSON.parse(json) as RocCommand;
  } catch {
    return { success: false, error: 'JSON parse error' };
  }

  const state: ScoreboardState = JSON.parse(JSON.stringify(current)); // deep clone

  switch (cmd.cmd) {
    case 'SET_SCORE': {
      const scoreA = typeof cmd['teamA'] === 'number' ? cmd['teamA'] as number : state.teamA.score;
      const scoreB = typeof cmd['teamB'] === 'number' ? cmd['teamB'] as number : state.teamB.score;
      state.teamA.score = Math.max(0, Math.min(99, scoreA));
      state.teamB.score = Math.max(0, Math.min(99, scoreB));
      return { success: true, state, response: 'OK' };
    }

    case 'SET_NAME': {
      const team = cmd['team'] as string;
      const name = (cmd['name'] as string) ?? '';
      if (team === 'A') state.teamA.name = name.slice(0, 10);
      else if (team === 'B') state.teamB.name = name.slice(0, 10);
      else return { success: false, error: `Unknown team: ${team}` };
      return { success: true, state, response: 'OK' };
    }

    case 'CLEAR':
      state.teamA = { name: '          ', score: 0 };
      state.teamB = { name: '          ', score: 0 };
      return { success: true, state, response: 'OK' };

    case 'PING':
      return { success: true, state, response: 'PONG' };

    default:
      return { success: false, error: `Unknown command: ${cmd.cmd}` };
  }
}
