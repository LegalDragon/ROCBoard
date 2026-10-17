import React from 'react';
import { SEGMENT_NAMES } from '../logic/font14segment';

interface Props {
  mask: number;
  size?: number;         // Base unit size in px (default 40)
  colorOn?: string;      // Segment color when lit (default amber)
  colorOff?: string;     // Segment color when dark
}

// SVG paths for each of the 14 segments in a normalized viewBox of 0 0 60 100
// Segments: a(top) b(upper-right) c(lower-right) d(bottom) e(lower-left) f(upper-left)
//           g(middle-left) h(middle-right) i(center-vertical-top) j(diag TL->C)
//           k(diag TR->C) l(center-vertical-bottom) m(diag C->BR) dp(decimal)
const SEGMENT_PATHS: Record<string, string> = {
  a:  'M 8,4  L 52,4  L 48,12 L 12,12 Z',           // top horizontal
  b:  'M 52,4  L 56,48 L 48,44 L 48,12 Z',           // upper-right vertical
  c:  'M 56,52 L 52,96 L 48,88 L 48,56 Z',           // lower-right vertical
  d:  'M 12,88 L 48,88 L 52,96 L 8,96 Z',            // bottom horizontal
  e:  'M 4,52  L 8,56  L 12,88 L 8,96 Z',            // lower-left vertical
  f:  'M 4,4   L 8,12  L 8,44  L 4,48 Z',            // upper-left vertical
  g:  'M 8,48  L 12,44 L 28,44 L 28,56 L 12,56 Z',  // middle-left horizontal
  h:  'M 32,44 L 48,44 L 52,48 L 48,56 L 32,56 Z',  // middle-right horizontal
  i:  'M 28,12 L 32,12 L 32,44 L 28,44 Z',           // upper-center vertical
  j:  'M 8,12  L 28,44 L 24,44 L 6,16 Z',            // diagonal TL to center
  k:  'M 32,44 L 52,12 L 54,16 L 36,44 Z',           // diagonal TR to center
  l:  'M 28,56 L 32,56 L 32,88 L 28,88 Z',           // lower-center vertical
  m:  'M 32,56 L 52,88 L 48,88 L 28,56 Z',           // diagonal center to BR
  dp: 'M 58,88 L 64,88 L 64,96 L 58,96 Z',           // decimal point
};

export const FourteenSegmentChar: React.FC<Props> = ({
  mask,
  size = 40,
  colorOn  = '#FFB300',
  colorOff = '#1a1a1a',
}) => {
  const scale = size / 60;
  const vw = 70 * scale;
  const vh = 100 * scale;

  return (
    <svg
      width={vw}
      height={vh}
      viewBox="0 0 70 100"
      style={{ display: 'inline-block' }}
    >
      {SEGMENT_NAMES.map((name, idx) => {
        const isOn = (mask & (1 << idx)) !== 0;
        const path = SEGMENT_PATHS[name];
        if (!path) return null;
        return (
          <path
            key={name}
            d={path}
            fill={isOn ? colorOn : colorOff}
            opacity={isOn ? 1.0 : 0.12}
          />
        );
      })}
    </svg>
  );
};
