#pragma once
// CommandProcessor — parses JSON command packets and dispatches to subsystems.
// Phase 4+ implementation.
//
// Command format (JSON over serial/Wi-Fi/BLE):
// { "cmd": "SET_SCORE", "teamA": 8, "teamB": 6 }
// { "cmd": "SET_NAME",  "team": "A", "name": "JOHN SMITH" }
// { "cmd": "CLEAR" }
// { "cmd": "PING" }

class DisplayEngine;

class CommandProcessor {
public:
    CommandProcessor();

    void init(DisplayEngine* display);

    // Process a null-terminated JSON string.
    // Returns true on success, false on parse/dispatch error.
    // TODO Phase 4: implement JSON parsing
    bool process(const char* json);

private:
    DisplayEngine* _display;
    bool           _initialized;
};
