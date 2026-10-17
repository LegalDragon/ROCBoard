import React, { useState } from 'react';

interface Props {
  onCommand?: (json: string) => void;
  lastResponse?: string;
}

const EXAMPLE_COMMANDS = [
  '{"cmd":"SET_SCORE","teamA":8,"teamB":6}',
  '{"cmd":"SET_NAME","team":"A","name":"JOHN"}',
  '{"cmd":"SET_NAME","team":"B","name":"DAVID"}',
  '{"cmd":"PING"}',
  '{"cmd":"CLEAR"}',
];

export const CommandConsole: React.FC<Props> = ({ onCommand, lastResponse }) => {
  const [input, setInput] = useState('');

  const handleSend = () => {
    const trimmed = input.trim();
    if (!trimmed) return;
    onCommand?.(trimmed);
    setInput('');
  };

  const handleExample = (cmd: string) => {
    setInput(cmd);
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
        minWidth: 480,
      }}
    >
      <div style={{ color: '#888', marginBottom: 8 }}>
        COMMAND CONSOLE
        <span style={{ color: '#444', marginLeft: 8, fontSize: 11 }}>
          (Phase 4: not wired to hardware yet)
        </span>
      </div>

      {/* Example buttons */}
      <div style={{ display: 'flex', flexWrap: 'wrap', gap: 4, marginBottom: 10 }}>
        {EXAMPLE_COMMANDS.map((cmd) => (
          <button
            key={cmd}
            onClick={() => handleExample(cmd)}
            style={{
              background: '#1a1a1a',
              color: '#AAA',
              border: '1px solid #444',
              borderRadius: 3,
              padding: '2px 6px',
              cursor: 'pointer',
              fontSize: 11,
            }}
          >
            {JSON.parse(cmd).cmd}
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
        placeholder='{"cmd":"PING"}'
        style={{
          width: '100%',
          background: '#111',
          color: '#EEE',
          border: '1px solid #444',
          borderRadius: 4,
          padding: 8,
          fontFamily: 'monospace',
          fontSize: 13,
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
            background: '#0a1a0a',
            border: '1px solid #2a4a2a',
            borderRadius: 4,
            color: '#7CFC00',
            fontSize: 12,
          }}
        >
          {lastResponse}
        </div>
      )}
    </div>
  );
};
