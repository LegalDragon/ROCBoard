# ROC Board

ESP32-S3 + WS2812B pickleball electronic scoreboard.

Displays team names (10 alpha chars each, 14-segment LED) and scores (2 numeric chars each, 7-segment LED) for live pickleball matches. Built for the ROC (Referee On Court) officiating system.

---

## Project Structure

```
ROCBoard/
├── firmware/        PlatformIO project (ESP32-S3, Arduino framework, FastLED)
├── simulator/       React + TypeScript web simulator (no hardware needed)
├── docs/            Wiring diagrams and command reference
└── .github/         CI workflows for firmware and simulator
```

---

## Phase 1 Quick Start (Windows)

### Prerequisites

1. Install **[VS Code](https://code.visualstudio.com/)**
2. Install the **[PlatformIO IDE](https://platformio.org/install/ide?install=vscode)** extension
3. Connect an **ESP32-S3-DevKitC-1** via USB

### Steps

1. Clone this repo:
   ```
   git clone https://github.com/LegalDragon/ROCBoard.git
   ```

2. Open the `firmware/` folder in VS Code:
   - File → Open Folder → select `ROCBoard/firmware`

3. **Build:** `Ctrl+Alt+B`

4. **Flash:** `Ctrl+Alt+U`

5. **Expected behavior:** The onboard RGB LED (GPIO 48) blinks:
   - RED (500ms) → GREEN (500ms) → BLUE (500ms) → OFF (500ms) → repeat

6. **Serial monitor:** `Ctrl+Alt+S` — 115200 baud
   - Output:
     ```
     ROCBoard Phase 1 starting...
     Profile: 1LED_TEST
     LED count: 1
     LED pin: 48
     LED driver initialized. Beginning blink cycle R→G→B→OFF
     [LED] RED
     [LED] GREEN
     ...
     ```

> GPIO 48 is the onboard WS2812B on the DevKitC-1 **N8R8** variant. No external wiring needed for Phase 1.
> See `docs/wiring-phase1.md` for external LED wiring if your board doesn't have the onboard LED.

---

## Simulator

No hardware needed — run the web simulator to see the scoreboard rendering:

```bash
cd simulator
npm install
npm run dev
```

Opens at `http://localhost:5173` — shows a live scoreboard with 14-segment LED characters.

The simulator uses the same segment bit definitions as the firmware (`font14segment.ts` ↔ `Font14Segment.h`).

---

## Development Phases

| Phase | Status     | Description |
|-------|------------|-------------|
| 1     | ✅ Complete | Single LED blink, project scaffold, simulator skeleton |
| 2     | 🔲 Planned  | DisplayEngine: 14-segment single character render |
| 3     | 🔲 Planned  | LayoutMapper: full 24-char scoreboard layout |
| 4     | 🔲 Planned  | CommandProcessor: JSON commands over serial |
| 5     | 🔲 Planned  | TransportManager: Wi-Fi REST API + OTA updates |
| 6     | 🔲 Planned  | BLE pucks: wireless score input devices |

---

## Hardware

| Component | Spec |
|-----------|------|
| MCU | ESP32-S3-DevKitC-1 N8R8 |
| LED type | WS2812B (addressable RGB) |
| LED count (Phase 1) | 1 (onboard) |
| LED count (full) | 308 (20×14-seg + 4×7-seg) |
| Data pin | GPIO 48 |
| Power (full board) | 5V / 5A |

---

## Future Architecture

The following interfaces are stubbed in `TransportManager.h` for future phases:

- **Wi-Fi (Phase 5):** REST API for score updates from a tablet/phone controller
- **BLE pucks (Phase 6):** Wireless Bluetooth score-input devices worn by referees
- **OTA updates (Phase 5):** Over-the-air firmware updates via Wi-Fi
- **Heartbeat service (Phase 5):** Watchdog + cloud connectivity monitoring
- **Mirror output (Phase 6):** Mirror scoreboard to secondary displays

---

## License

MIT
