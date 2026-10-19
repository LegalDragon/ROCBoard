#pragma once
#include <stdint.h>
// TransportManager — manages all communication transports.
// Phase 4: USB serial (newline-delimited JSON).
// Phase 5+: Wi-Fi REST, BLE pucks, OTA, heartbeat.
//
// Future interface stubs are declared in firmware/src/interfaces/.

class CommandProcessor;

class TransportManager {
public:
    TransportManager();

    void init(CommandProcessor* processor);

    // Poll all active transports for incoming data.
    // Call from loop(). Non-blocking.
    void poll();

    // Enable/disable individual transports.
    void enableSerial(bool enable);
    void enableWifi(bool enable);       // Phase 5+
    void enableBluetooth(bool enable);  // Phase 6+

private:
    CommandProcessor* _processor;
    bool _initialized;
    bool _serialEnabled;
    bool _wifiEnabled;
    bool _bluetoothEnabled;

    // Serial framing buffer — accumulate chars until '\n'
    static constexpr uint16_t SERIAL_BUF_SIZE = 512;
    char     _serialBuf[SERIAL_BUF_SIZE];
    uint16_t _serialLen;

    void _pollSerial();
};
