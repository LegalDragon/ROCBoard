#include "Diagnostics.h"
// TODO Phase 2: Implement Diagnostics
// Provides POST, logging, and health monitoring for the ROCBoard firmware.

#ifndef NATIVE_TEST
#include <Arduino.h>
#endif

Diagnostics::Diagnostics()
    : _errorCount(0)
{}

DiagResult Diagnostics::runPost(LedDriver* driver) {
    (void)driver;
    // TODO Phase 2: cycle each LED through red/green/blue at low brightness
    return DiagResult::OK;
}

void Diagnostics::log(const char* level, const char* message) {
#ifndef NATIVE_TEST
    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);
#else
    (void)level;
    (void)message;
#endif
}

void Diagnostics::reportStatus() {
#ifndef NATIVE_TEST
    Serial.print("[DIAG] Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println("s");
    Serial.print("[DIAG] Errors: ");
    Serial.println(_errorCount);
    // TODO Phase 2: report heap free (ESP.getFreeHeap()), LED count, etc.
#endif
}

uint32_t Diagnostics::recordError(const char* context) {
    _errorCount++;
    log("ERROR", context);
    return _errorCount;
}
