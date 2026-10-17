#pragma once
#include <stdint.h>
// Diagnostics — system health checks and error reporting.
// Phase 2+ implementation.

enum class DiagResult {
    OK,
    WARN,
    ERROR
};

class LedDriver;

class Diagnostics {
public:
    Diagnostics();

    // Run power-on self-test (POST): blinks each LED once to verify hardware.
    // TODO Phase 2: implement POST
    DiagResult runPost(LedDriver* driver);

    // Log a message to Serial (and future: cloud logging).
    // TODO Phase 2: integrate with TransportManager for remote logging
    void log(const char* level, const char* message);

    // Report current heap free, uptime, LED count to Serial.
    // TODO Phase 2: implement status dump
    void reportStatus();

    // Increment and return error counter.
    uint32_t recordError(const char* context);

private:
    uint32_t _errorCount;
};
