import React, { useState, useEffect, useRef } from 'react';
import { Scoreboard } from './components/Scoreboard';
import { CommandConsole } from './components/CommandConsole';
import { FourteenSegmentChar } from './components/FourteenSegmentChar';
import { processCommand } from './logic/commandProcessor';
import { charToMask } from './logic/font14segment';
import type { ScoreboardState } from './types/roc';

// Phase 1 demo state: JOHN/MIKE vs DAVID/PETER, scores 8 vs 6
const INITIAL_STATE: ScoreboardState = {
  teamA: { name: 'JOHN MIKE', score: 8 },
  teamB: { name: 'DAVIDPETER', score: 6 },
  serving: 'A',
  gameNumber: 1,
};

// Phase 2 Test Mode: cycle A-Z then 0-9
const TEST_CYCLE = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
const TEST_INTERVAL_MS = 500;

export default function App() {
  const [state, setState] = useState<ScoreboardState>(INITIAL_STATE);
  const [lastResponse, setLastResponse] = useState<string>('');

  // Phase 2 Test Mode state
  const [testMode, setTestMode] = useState(false);
  const [testCharIdx, setTestCharIdx] = useState(0);
  const testTimerRef = useRef<ReturnType<typeof setInterval> | null>(null);

  // Start / stop test mode
  useEffect(() => {
    if (testMode) {
      testTimerRef.current = setInterval(() => {
        setTestCharIdx(idx => (idx + 1) % TEST_CYCLE.length);
      }, TEST_INTERVAL_MS);
    } else {
      if (testTimerRef.current) clearInterval(testTimerRef.current);
      setTestCharIdx(0);
    }
    return () => {
      if (testTimerRef.current) clearInterval(testTimerRef.current);
    };
  }, [testMode]);

  const handleCommand = (json: string) => {
    const result = processCommand(json, state);
    if (result.success && result.state) {
      setState(result.state);
      setLastResponse(`\u2713 ${result.response ?? 'OK'}`);
    } else {
      setLastResponse(`\u2717 Error: ${result.error ?? 'unknown'}`);
    }
  };

  const testChar = TEST_CYCLE[testCharIdx];
  const testMask = charToMask(testChar);

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

      {/* Phase 2: Test Mode panel */}
      <div
        style={{
          background: '#0d0d0d',
          border: '1px solid #333',
          borderRadius: 6,
          padding: '16px 24px',
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          gap: 12,
        }}
      >
        <div style={{ color: '#888', fontSize: 11, letterSpacing: 2 }}>
          PHASE 2 — SINGLE CHARACTER TEST MODE
        </div>

        {/* SVG segment order matches firmware: bit0=a, bit1=b, ..., bit13=dp */}
        {/* on = team color (white default), off = dim gray #2a2a2a */}
        <FourteenSegmentChar
          mask={testMode ? testMask : 0}
          size={80}
          colorOn="#FFFFFF"
          colorOff="#2a2a2a"
        />

        {testMode && (
          <div style={{ color: '#FFB300', fontSize: 20, letterSpacing: 4 }}>
            {testChar}
          </div>
        )}

        <button
          onClick={() => setTestMode(m => !m)}
          style={{
            background: testMode ? '#7B1FA2' : '#1B5E20',
            color: '#FFF',
            border: 'none',
            borderRadius: 4,
            padding: '6px 18px',
            cursor: 'pointer',
            fontFamily: 'monospace',
            fontSize: 12,
            letterSpacing: 1,
          }}
        >
          {testMode ? 'STOP TEST MODE' : 'START TEST MODE (A\u2192Z\u21920\u21929)'}
        </button>
      </div>

      <CommandConsole onCommand={handleCommand} lastResponse={lastResponse} />

      <div style={{ color: '#333', fontSize: 11 }}>
        Phase 2 — 14-segment single character rendering | Hardware: ESP32-S3 + WS2812B GPIO1
      </div>
    </div>
  );
}
