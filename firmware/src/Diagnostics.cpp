#include "Diagnostics.h"
#include "LedDriver.h"

#ifndef NATIVE_TEST
#include <Arduino.h>
#endif

Diagnostics::Diagnostics()
    : _errorCount(0)
{}

DiagResult Diagnostics::runPost(LedDriver* driver) {
    if (!driver) return DiagResult::ERROR;

    log("INFO", "POST starting — cycling all LEDs R/G/B");

#ifndef NATIVE_TEST
    uint16_t count = driver->ledCount();
    if (count == 0) return DiagResult::WARN;

    // Flash all LEDs red → green → blue → off at low brightness
    const uint8_t LEVEL = 30;
    const unsigned long STEP_MS = 150;

    driver->setBrightness(LEVEL);

    uint8_t colors[3][3] = {
        {LEVEL, 0, 0},  // red
        {0, LEVEL, 0},  // green
        {0, 0, LEVEL},  // blue
    };

    for (int ci = 0; ci < 3; ci++) {
        for (uint16_t i = 0; i < count; i++) {
            driver->setPixel(i, colors[ci][0], colors[ci][1], colors[ci][2]);
        }
        driver->show();
        delay(STEP_MS); // Only used in POST — blocking is acceptable here
    }
    driver->clear();
    log("INFO", "POST complete — OK");
#endif

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
    Serial.print("s | FreeHeap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.print(" | Errors: ");
    Serial.println(_errorCount);
#endif
}

uint32_t Diagnostics::recordError(const char* context) {
    _errorCount++;
    log("ERROR", context);
    return _errorCount;
}
