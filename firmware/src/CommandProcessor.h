#pragma once
#include <stdint.h>
// CommandProcessor — parses newline-delimited JSON commands and updates ScoreboardState.
// Phase 4+ implementation. Uses ArduinoJson for parsing.
//
// Supported commands (spec-compliant):
//   set_display  — set top/bottom player names, scores, colors
//   swap_teams   — swap top and bottom teams
//   brightness   — set global LED brightness (value: 0-255)
//   self_test    — trigger POST animation
//   ping         — connectivity check → "PONG"
//   status       — report current state as JSON
//   clear        — blank the display

class DisplayEngine;
struct ScoreboardState;

class CommandProcessor {
public:
    CommandProcessor();

    // Initialize with display engine and state pointers.
    void init(DisplayEngine* display, ScoreboardState* state);

    // Register a callback that fires when state changes (e.g. to trigger re-render).
    void setOnStateChanged(void (*cb)(void));

    // Register a callback for self_test command.
    void setOnSelfTest(void (*cb)(void));

    // Process a null-terminated JSON string.
    // Returns true on success. Writes JSON response to `responseBuf` (max `responseLen` bytes).
    bool process(const char* json, char* responseBuf, uint16_t responseLen);

private:
    DisplayEngine*  _display;
    ScoreboardState* _state;
    void (*_onStateChanged)(void);
    void (*_onSelfTest)(void);
    bool _initialized;

    void _notifyStateChanged();
    void _buildStatusResponse(char* buf, uint16_t len);
};
