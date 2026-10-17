#include "TransportManager.h"
// TODO Phase 5: Implement TransportManager
// Manages serial, Wi-Fi, and BLE transports.
// Each transport reads bytes, assembles JSON frames, and calls CommandProcessor::process().

TransportManager::TransportManager()
    : _processor(nullptr)
    , _initialized(false)
    , _serialEnabled(false)
    , _wifiEnabled(false)
    , _bluetoothEnabled(false)
{}

void TransportManager::init(CommandProcessor* processor) {
    _processor   = processor;
    _initialized = true;
    // TODO Phase 5: initialize serial framing, Wi-Fi stack, BLE stack
}

void TransportManager::poll() {
    // TODO Phase 5: check each enabled transport for incoming data
}

void TransportManager::enableSerial(bool enable) {
    _serialEnabled = enable;
    // TODO Phase 5: set up Serial2 or stdin framing
}

void TransportManager::enableWifi(bool enable) {
    _wifiEnabled = enable;
    // TODO Phase 5: start/stop Wi-Fi AP or STA mode
}

void TransportManager::enableBluetooth(bool enable) {
    _bluetoothEnabled = enable;
    // TODO Phase 5: start/stop BLE server and puck pairing
}
