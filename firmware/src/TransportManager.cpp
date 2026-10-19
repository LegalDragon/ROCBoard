#include "TransportManager.h"
#include "CommandProcessor.h"
#include <string.h>

#ifndef NATIVE_TEST
#include <Arduino.h>
#endif

TransportManager::TransportManager()
    : _processor(nullptr)
    , _initialized(false)
    , _serialEnabled(false)
    , _wifiEnabled(false)
    , _bluetoothEnabled(false)
    , _serialLen(0)
{
    memset(_serialBuf, 0, sizeof(_serialBuf));
}

void TransportManager::init(CommandProcessor* processor) {
    _processor   = processor;
    _initialized = true;
}

void TransportManager::enableSerial(bool enable) {
    _serialEnabled = enable;
    _serialLen     = 0; // reset framing buffer on toggle
}

void TransportManager::enableWifi(bool enable) {
    _wifiEnabled = enable;
    // TODO Phase 5: start/stop Wi-Fi AP/STA mode
}

void TransportManager::enableBluetooth(bool enable) {
    _bluetoothEnabled = enable;
    // TODO Phase 6: start/stop BLE server and puck pairing
}

void TransportManager::poll() {
    if (!_initialized) return;
    if (_serialEnabled) _pollSerial();
    // TODO Phase 5: _pollWifi();
    // TODO Phase 6: _pollBluetooth();
}

void TransportManager::_pollSerial() {
#ifndef NATIVE_TEST
    if (!_processor) return;

    char responseBuf[256];

    while (Serial.available()) {
        char c = (char)Serial.read();

        if (c == '\n' || c == '\r') {
            if (_serialLen == 0) continue; // skip empty lines

            _serialBuf[_serialLen] = '\0';
            bool ok = _processor->process(_serialBuf, responseBuf, sizeof(responseBuf));
            Serial.println(responseBuf);
            (void)ok;
            _serialLen = 0;
        } else if (_serialLen < SERIAL_BUF_SIZE - 1) {
            _serialBuf[_serialLen++] = c;
        } else {
            // Buffer overflow — discard and reset
            _serialLen = 0;
            Serial.println("{\"status\":\"error\",\"error\":\"input too long\"}");
        }
    }
#endif
}
