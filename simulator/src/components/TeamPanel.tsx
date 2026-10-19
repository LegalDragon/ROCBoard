import React from 'react';
import { FourteenSegmentChar } from './FourteenSegmentChar';
import { NumericChar } from './NumericChar';
import { charToMask } from '../logic/font14segment';
import type { TeamState } from '../types/roc';

interface Props {
  team: TeamState;
  isTop?: boolean;     // determines label
  charSize?: number;
}

export const TeamPanel: React.FC<Props> = ({ team, isTop = true, charSize = 36 }) => {
  // Pad/truncate name to 10 characters
  const display = team.name.toUpperCase().padEnd(10, ' ').slice(0, 10);
  const scoreDigit10 = Math.floor(team.score / 10);
  const scoreDigit1  = team.score % 10;
  const color = team.color ?? (isTop ? '#FF4444' : '#4488FF');

  return (
    <div style={{ display: 'flex', alignItems: 'center', gap: 4 }}>
      {/* 10 alpha chars for team name */}
      {Array.from(display).map((c, i) => (
        <FourteenSegmentChar
          key={i}
          mask={charToMask(c)}
          size={charSize}
          colorOn={color}
        />
      ))}

      {/* Divider */}
      <div style={{ width: 12 }} />

      {/* 2 numeric chars for score */}
      <NumericChar digit={scoreDigit10} size={charSize * 1.2} colorOn={color} />
      <NumericChar digit={scoreDigit1}  size={charSize * 1.2} colorOn={color} />
    </div>
  );
};
