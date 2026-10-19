import React, { useState } from 'react';

interface Props {
  onCommand?: (json: string) => void;
  lastResponse?: string;
}

const EXAMPLE_COMMANDS = [
  {
    label: 'set_display',
    json: '{"cmd":"set_display","top":{"players":["JOHN","MIKE"],"score":8,"color":"#FF4400"},"bottom":{"players":["DAVID","PETER"],"score":6,"color":"#0066FF"}}',
  },
  { label: 'swap_teams', json: '{"cmd":"swap_teams"}' },
  { label: 'brightness', json: '{"cmd":"brightness","value":180}' },
  { label: 'self_test',  json: '{"cmd":"self_test"}' },
  { label: 'SET_SCORE',  json: '{"cmd":"SET_SCORE","teamA":11,"teamB":9}' },
  { label: 'SET_NAME',   json: '{"cmd":"SET_NAME","team":"A","name":"SYNTHIA"}' },
  { label: 'ping',       json: '{"cmd":"ping"}' },
  { label: 'clear',      json: '{"cmd":"clear"}' },
];

export const CommandConsole: React.FC<Props> = ({ onCommand, lastResponse }) => {
  const [input, setInput] = useState('');

  const handleSend = () => {
    const trimmed = input.trim();
    if (!trimmed) return;
    onCommand?.(trimmed);
    setInput('');
  };

  return (
    <div
      style={{
        background: '#0d0d0d',
        border: '1px solid #333',
        borderRadius: 6,
        padding: 16,
        fontFamily: 'monospace',
        fontSize: 13,
        minWidth: 520,
        maxWidth: 680,
      }}
    >
      <div style={{ color: '#888', marginBottom: 8 }}>
        COMMAND CONSOLE
        <span style={{ color: '#444', marginLeft: 8, fontSize: 11 }}>
          (JSON over serial — same format as hardware)
        </span>
      </div>

      {/* Example buttons */}
      <div style={{ display: 'flex', flexWrap: 'wrap', gap: 4, marginBottom: 10 }}>
        {EXAMPLE_COMMANDS.map((ex) => (
          <button
            key={ex.label}
            onClick={() => setInput(ex.json)}
            style={{
              background: '#1a1a1a',
              color: '#AAA',
              border: '1px solid #444',
              borderRadius: 3,
              padding: '2px 8px',
              cursor: 'pointer',
              fontSize: 11,
            }}
          >
            {ex.label}
          </button>
        ))}
      </div>

      {/* Input area */}
      <textarea
        value={input}
        onChange={(e) => setInput(e.target.value)}
        onKeyDown={(e) => {
          if (e.key === 'Enter' && (e.ctrlKey || e.metaKey)) handleSend();
        }}
        rows={3}
        placeholder='{"cmd":"ping"}'
        style={{
          width: '100%',
          background: '#111',
          color: '#EEE',
          border: '1px solid #444',
          borderRadius: 4,
          padding: 8,
          fontFamily: 'monospace',
          fontSize: 12,
          resize: 'vertical',
          boxSizing: 'border-box',
        }}
      />

      <div style={{ display: 'flex', justifyContent: 'space-between', marginTop: 8, alignItems: 'center' }}>
        <span style={{ color: '#555', fontSize: 11 }}>Ctrl+Enter to send</span>
        <button
          onClick={handleSend}
          style={{
            background: '#1a3a1a',
            color: '#4CAF50',
            border: '1px solid #4CAF50',
            borderRadius: 4,
            padding: '6px 16px',
            cursor: 'pointer',
            fontFamily: 'monospace',
            fontSize: 13,
          }}
        >
          SEND
        </button>
      </div>

      {/* Response area */}
      {lastResponse && (
        <div
          style={{
            marginTop: 10,
            padding: 8,
            background: lastResponse.startsWith('✗') ? '#1a0a0a' : '#0a1a0a',
            border: `1px solid ${lastResponse.startsWith('✗') ? '#4a2a2a' : '#2a4a2a'}`,
            borderRadius: 4,
            color: lastResponse.startsWith('✗') ? '#FF6666' : '#7CFC00',
            fontSize: 12,
          }}
        >
          {lastResponse}
        </div>
      )}
    </div>
  );
};
