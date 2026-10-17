import React, { useState } from 'react';
import { Scoreboard } from './components/Scoreboard';
import { CommandConsole } from './components/CommandConsole';
import { processCommand } from './logic/commandProcessor';
import type { ScoreboardState } from './types/roc';

// Phase 1 demo state: JOHN/MIKE vs DAVID/PETER, scores 8 vs 6
const INITIAL_STATE: ScoreboardState = {
  teamA: { name: 'JOHN MIKE', score: 8 },
  teamB: { name: 'DAVIDPETER', score: 6 },
  serving: 'A',
  gameNumber: 1,
};

export default function App() {
  const [state, setState] = useState<ScoreboardState>(INITIAL_STATE);
  const [lastResponse, setLastResponse] = useState<string>('');

  const handleCommand = (json: string) => {
    const result = processCommand(json, state);
    if (result.success && result.state) {
      setState(result.state);
      setLastResponse(`✓ ${result.response ?? 'OK'}`);
    } else {
      setLastResponse(`✗ Error: ${result.error ?? 'unknown'}`);
    }
  };

  return (
    <div
      style={{
        minHeight: '100vh',
        background: '#050505',
        color: '#EEE',
        fontFamily: 'monospace',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        justifyContent: 'center',
        gap: 32,
        padding: 32,
      }}
    >
      <h1 style={{ color: '#FFB300', letterSpacing: 4, fontSize: 18, margin: 0 }}>
        ROC BOARD SIMULATOR
      </h1>

      <Scoreboard state={state} />

      <CommandConsole onCommand={handleCommand} lastResponse={lastResponse} />

      <div style={{ color: '#333', fontSize: 11 }}>
        Phase 1 — Simulator skeleton | Hardware: ESP32-S3 + WS2812B
      </div>
    </div>
  );
}
