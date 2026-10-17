import React from 'react';

interface Props {
  digit: number;   // 0-9 (or -1 for blank)
  size?: number;
  colorOn?: string;
  colorOff?: string;
}

// 7-segment display for numeric-only positions (score digits)
// Segments: a(top) b(upper-right) c(lower-right) d(bottom) e(lower-left) f(upper-left) g(middle)
const SEV_SEG_MASKS: Record<number, number> = {
  0: 0b0111111,
  1: 0b0000110,
  2: 0b1011011,
  3: 0b1001111,
  4: 0b1100110,
  5: 0b1101101,
  6: 0b1111101,
  7: 0b0000111,
  8: 0b1111111,
  9: 0b1101111,
};

const SEV_PATHS: string[] = [
  'M 8,4  L 52,4  L 48,12 L 12,12 Z',       // a top
  'M 52,4  L 56,48 L 48,44 L 48,12 Z',       // b upper-right
  'M 56,52 L 52,96 L 48,88 L 48,56 Z',       // c lower-right
  'M 12,88 L 48,88 L 52,96 L 8,96 Z',        // d bottom
  'M 4,52  L 8,56  L 12,88 L 8,96 Z',        // e lower-left
  'M 4,4   L 8,12  L 8,44  L 4,48 Z',        // f upper-left
  'M 8,48  L 12,44 L 48,44 L 52,48 L 48,56 L 12,56 Z',  // g middle
];

export const NumericChar: React.FC<Props> = ({
  digit,
  size = 40,
  colorOn  = '#FFB300',
  colorOff = '#1a1a1a',
}) => {
  const mask = digit >= 0 && digit <= 9 ? SEV_SEG_MASKS[digit] : 0;
  const scale = size / 60;

  return (
    <svg
      width={60 * scale}
      height={100 * scale}
      viewBox="0 0 60 100"
      style={{ display: 'inline-block' }}
    >
      {SEV_PATHS.map((path, idx) => {
        const isOn = (mask & (1 << (6 - idx))) !== 0;
        return (
          <path
            key={idx}
            d={path}
            fill={isOn ? colorOn : colorOff}
            opacity={isOn ? 1.0 : 0.12}
          />
        );
      })}
    </svg>
  );
};
