import React, { useState, useEffect, useRef } from 'react';
import { Scoreboard } from './components/Scoreboard';
import { CommandConsole } from './components/CommandConsole';
import { FourteenSegmentChar } from './components/FourteenSegmentChar';
import { processCommand } from './logic/commandProcessor';
import { charToMask } from './logic/font14segment';
import type { ScoreboardState } from './types/roc';

const INITIAL_STATE: ScoreboardState = {
  teamA: { name: 'JOHN MIKE ', score: 8,  color: '#FF4400' },
  teamB: { name: 'DAVID PTR ', score: 6,  color: '#0066FF' },
  brightness: 50,
  swapped: false,
  serving: 'A',
  gameNumber: 1,
};

// Phase 2 Test Mode: cycle A-Z then 0-9
const TEST_CYCLE = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
const TEST_INTERVAL_MS = 500;

export default function App() {
  const [state, setState] = useState<ScoreboardState>(INITIAL_STATE);
  const [lastResponse, setLastResponse] = useState<string>('');
  const [selfTestActive, setSelfTestActive] = useState(false);

  // Phase 2 Test Mode state
  const [testMode, setTestMode] = useState(false);
  const [testCharIdx, setTestCharIdx] = useState(0);
  const testTimerRef = useRef<ReturnType<typeof setInterval> | null>(null);

  // Self-test flash animation (cycles all segments briefly)
  const selfTestTimerRef = useRef<ReturnType<typeof setInterval> | null>(null);
  const [selfTestSegIdx, setSelfTestSegIdx] = useState(0);

  // Start / stop character test mode
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

  // Self-test animation (sequential segment illumination)
  useEffect(() => {
    if (selfTestActive) {
      selfTestTimerRef.current = setInterval(() => {
        setSelfTestSegIdx(s => {
          if (s >= 15) {
            setSelfTestActive(false);
            return 0;
          }
          return s + 1;
        });
      }, 80);
    } else {
      if (selfTestTimerRef.current) clearInterval(selfTestTimerRef.current);
    }
    return () => {
      if (selfTestTimerRef.current) clearInterval(selfTestTimerRef.current);
    };
  }, [selfTestActive]);

  const handleCommand = (json: string) => {
    const result = processCommand(json, state);
    if (result.success && result.state) {
      setState(result.state);
      // Trigger self-test animation if command was self_test
      if (json.includes('self_test')) setSelfTestActive(true);
      setLastResponse(`✓ ${result.response ?? 'OK'}`);
    } else {
      setLastResponse(`✗ Error: ${result.error ?? 'unknown'}`);
    }
  };

  const testChar = TEST_CYCLE[testCharIdx];
  const testMask = charToMask(testChar);

  // Self-test mask: lights one segment at a time, then all, then off
  const selfTestMask = selfTestActive
    ? (selfTestSegIdx < 14 ? (1 << selfTestSegIdx) : (selfTestSegIdx === 14 ? 0x3FFF : 0))
    : 0;

  // Brightness as opacity for simulator
  const brightnessScale = Math.max(0.15, state.brightness / 255);

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

      {/* Main scoreboard */}
      <div style={{ opacity: brightnessScale }}>
        <Scoreboard state={state} />
      </div>

      {/* Brightness indicator */}
      <div style={{ color: '#555', fontSize: 11 }}>
        brightness: {state.brightness} / 255
        {state.swapped && <span style={{ color: '#FFB300', marginLeft: 12 }}>⇅ SWAPPED</span>}
      </div>

      {/* Phase 2: Single character test panel */}
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
          SINGLE CHARACTER TEST MODE
        </div>

        <FourteenSegmentChar
          mask={selfTestActive ? selfTestMask : testMode ? testMask : 0}
          size={80}
          colorOn="#FFFFFF"
          colorOff="#2a2a2a"
        />

        {testMode && !selfTestActive && (
          <div style={{ color: '#FFB300', fontSize: 20, letterSpacing: 4 }}>
            {testChar}
          </div>
        )}

        {selfTestActive && (
          <div style={{ color: '#00BFFF', fontSize: 12 }}>
            SELF TEST — seg {selfTestSegIdx < 14 ? selfTestSegIdx : selfTestSegIdx === 14 ? 'ALL' : 'OFF'}
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
          {testMode ? 'STOP TEST MODE' : 'START TEST MODE (A→Z→0→9)'}
        </button>
      </div>

      <CommandConsole onCommand={handleCommand} lastResponse={lastResponse} />

      <div style={{ color: '#333', fontSize: 11 }}>
        Phase 3/4/5 — Full scoreboard | JSON serial protocol | Hardware: ESP32-S3 + WS2812B GPIO1
      </div>
    </div>
  );
}
