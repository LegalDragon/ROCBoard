// ROCBoard shared type definitions

export interface TeamState {
  name: string;       // Up to 10 chars (alpha chars on scoreboard)
  score: number;      // 0-99
  color?: string;     // CSS color e.g. "#FF0000"
}

export interface ScoreboardState {
  teamA: TeamState;   // top team
  teamB: TeamState;   // bottom team
  brightness: number; // 0-255
  swapped: boolean;
  serving?: 'A' | 'B' | null;
  gameNumber?: number;
}

// ─── Command types ────────────────────────────────────────────────────────

export interface RocCommand {
  cmd: string;
  [key: string]: unknown;
}

export interface SetDisplayCommand extends RocCommand {
  cmd: 'set_display';
  top: {
    players: string[];
    score: number;
    color?: string;
  };
  bottom: {
    players: string[];
    score: number;
    color?: string;
  };
}

export interface SwapTeamsCommand extends RocCommand {
  cmd: 'swap_teams';
}

export interface BrightnessCommand extends RocCommand {
  cmd: 'brightness';
  value: number;
}

export interface SelfTestCommand extends RocCommand {
  cmd: 'self_test';
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

export interface ClearCommand extends RocCommand { cmd: 'CLEAR'; }
export interface PingCommand  extends RocCommand { cmd: 'ping';  }

// 14-segment character position type (0-23)
export type CharPosition = number;
