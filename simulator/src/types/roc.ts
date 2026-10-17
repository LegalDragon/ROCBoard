// ROCBoard shared type definitions

export interface TeamState {
  name: string;       // Up to 10 chars (alpha chars on scoreboard)
  score: number;      // 0-99
}

export interface ScoreboardState {
  teamA: TeamState;
  teamB: TeamState;
  serving?: 'A' | 'B' | null;
  gameNumber?: number;
}

export interface RocCommand {
  cmd: string;
  [key: string]: unknown;
}

export interface SetScoreCommand extends RocCommand {
  cmd: 'SET_SCORE';
  teamA: number;
  teamB: number;
}

export interface SetNameCommand extends RocCommand {
  cmd: 'SET_NAME';
  team: 'A' | 'B';
  name: string;
}

export interface ClearCommand extends RocCommand {
  cmd: 'CLEAR';
}

export interface PingCommand extends RocCommand {
  cmd: 'PING';
}

// 14-segment character position type
export type CharPosition = number; // 0-23
