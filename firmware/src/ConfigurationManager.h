#pragma once
#include <stdint.h>
// ConfigurationManager — loads and persists hardware + network configuration.
// Phase 5+ implementation.

struct RocConfig {
    char     ssid[64];
    char     wifiPassword[64];
    char     apName[32];
    uint16_t ledCount;
    uint8_t  ledPin;
    uint8_t  brightness;
    // TODO Phase 5: add more config fields (BLE name, OTA password, etc.)
};

class ConfigurationManager {
public:
    ConfigurationManager();

    // Load config from NVS (non-volatile storage) or defaults.
    // TODO Phase 5: implement NVS read
    void load();

    // Save current config to NVS.
    // TODO Phase 5: implement NVS write
    void save();

    // Returns a reference to the current config.
    const RocConfig& config() const;

    // Reset to factory defaults.
    void reset();

private:
    RocConfig _config;
    bool      _loaded;
};
