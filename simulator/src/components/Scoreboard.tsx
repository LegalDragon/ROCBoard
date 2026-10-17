import React from 'react';
import { TeamPanel } from './TeamPanel';
import type { ScoreboardState } from '../types/roc';

interface Props {
  state: ScoreboardState;
}

export const Scoreboard: React.FC<Props> = ({ state }) => {
  return (
    <div
      style={{
        background: '#0a0a0a',
        border: '2px solid #333',
        borderRadius: 8,
        padding: '24px 32px',
        display: 'inline-flex',
        flexDirection: 'column',
        gap: 16,
        boxShadow: '0 0 40px rgba(255,179,0,0.15)',
      }}
    >
      <div style={{ color: '#555', fontSize: 11, letterSpacing: 2, marginBottom: 4 }}>
        ROC BOARD — LIVE SCOREBOARD
      </div>

      <TeamPanel team={state.teamA} charSize={36} />

      {/* Serving indicator */}
      {state.serving && (
        <div style={{ color: '#FFB300', fontSize: 12, paddingLeft: 4 }}>
          {state.serving === 'A' ? '▶ Team A serving' : '▶ Team B serving'}
        </div>
      )}

      <TeamPanel team={state.teamB} charSize={36} />
    </div>
  );
};
