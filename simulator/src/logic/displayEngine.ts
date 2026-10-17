// DisplayEngine — maps ScoreboardState to per-character segment masks.
// Phase 2+ will expand this to handle animation, brightness, etc.

import { charToMask } from './font14segment';
import type { ScoreboardState } from '../types/roc';

export interface DisplayFrame {
  // 24 characters: positions 0-9 = team A name, 10-19 = team B name,
  // 20-21 = score A (2 digits), 22-23 = score B (2 digits)
  chars: number[]; // 24 segment masks
}

export function stateToFrame(state: ScoreboardState): DisplayFrame {
  const chars: number[] = new Array(24).fill(0);

  // Team A name — left-pad or truncate to 10 chars
  const nameA = state.teamA.name.toUpperCase().padEnd(10, ' ').slice(0, 10);
  for (let i = 0; i < 10; i++) {
    chars[i] = charToMask(nameA[i]);
  }

  // Team B name — positions 10-19
  const nameB = state.teamB.name.toUpperCase().padEnd(10, ' ').slice(0, 10);
  for (let i = 0; i < 10; i++) {
    chars[10 + i] = charToMask(nameB[i]);
  }

  // Score A — positions 20-21 (two digits, zero-padded)
  const scoreA = Math.max(0, Math.min(99, state.teamA.score));
  chars[20] = charToMask(String(Math.floor(scoreA / 10)));
  chars[21] = charToMask(String(scoreA % 10));

  // Score B — positions 22-23
  const scoreB = Math.max(0, Math.min(99, state.teamB.score));
  chars[22] = charToMask(String(Math.floor(scoreB / 10)));
  chars[23] = charToMask(String(scoreB % 10));

  return { chars };
}
