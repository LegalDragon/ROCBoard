#pragma once
// TransportManager — manages all communication transports.
// Phase 5+ implementation.
//
// Supported transports (future phases):
//   - Serial (Phase 4, for testing)
//   - Wi-Fi / HTTP REST (Phase 5)
//   - Bluetooth Low Energy (BLE) pucks (Phase 6)
//   - OTA update manager (Phase 5)

class CommandProcessor;

// Future interfaces — not yet implemented
// Future: IWifiTransport
// Future: IBluetoothTransport (BLE pucks)
// Future: IRestApiTransport
// Future: IOtaManager
// Future: IHeartbeatService
// Future: IMirrorOutput (mirror display to secondary devices)

class TransportManager {
public:
    TransportManager();

    void init(CommandProcessor* processor);

    // Poll all active transports for incoming data.
    // Call from loop(). Non-blocking.
    // TODO Phase 5: implement transport polling
    void poll();

    // Enable/disable individual transports.
    // TODO Phase 5: implement transport enable/disable
    void enableSerial(bool enable);
    void enableWifi(bool enable);
    void enableBluetooth(bool enable);

private:
    CommandProcessor* _processor;
    bool _initialized;
    bool _serialEnabled;
    bool _wifiEnabled;
    bool _bluetoothEnabled;
    // TODO Phase 5: add transport objects
};
