# ROCBoard Phase 1 — Wiring Guide

## Overview

Phase 1 uses a **single WS2812B LED** to verify the firmware, LED driver, and blink cycle.

On the **ESP32-S3-DevKitC-1 N8R8**, GPIO 48 is the onboard addressable RGB LED.
**Phase 1 requires no external wiring.** Just plug in the board and flash.

---

## Onboard LED (Zero-wire test)

```
ESP32-S3-DevKitC-1 (N8R8)
┌──────────────────────┐
│                      │
│  GPIO 48 ────────────┼──→ WS2812B DIN (onboard)
│  3.3V internal       │    VCC (onboard)
│  GND internal        │    GND (onboard)
│                      │
└──────────────────────┘
```

Expected behavior: LED cycles RED → GREEN → BLUE → OFF, 500ms each.

---

## External LED Wiring (if using a separate WS2812B)

```
ESP32-S3-DevKitC-1          WS2812B LED Module
┌──────────────────┐        ┌──────────────────┐
│                  │        │                  │
│  GPIO 48 ────────┼──330Ω──┤ DIN              │
│                  │        │                  │
│  5V (USB VBUS)   ├────────┤ VCC (+5V)        │
│  or external 5V  │        │                  │
│  GND             ├────────┤ GND              │
│                  │        │                  │
└──────────────────┘        └──────────────────┘
```

### Notes

1. **330Ω resistor on data line** — protects the ESP32 output from ringing on long wires.
   Use 300–500Ω. Can be omitted for bench testing with <30cm wire.

2. **3.3V logic compatibility** — WS2812B spec requires 0.7×VCC ≈ 3.5V for logic HIGH when
   powered at 5V. ESP32-S3 outputs 3.3V. This *usually* works in practice but is out of spec.
   For reliable operation at scale, add a 3.3V→5V level shifter (e.g., 74AHCT125) on the data line.

3. **Power supply** — Use the 5V USB VBUS pin for up to ~10 LEDs at full white.
   For the full 308-LED scoreboard, use a dedicated 5V/5A power supply.
   Connect GND of external supply to ESP32 GND.

---

## Phase 2+ Expansion

As LED count grows, add:
- Capacitor (100–1000µF, 6.3V+) across 5V/GND near the LED strip
- Fuse (3A fast-blow) on the 5V supply line
- Level shifter for reliable 3.3V→5V data conversion
