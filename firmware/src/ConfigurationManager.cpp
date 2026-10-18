#include "ConfigurationManager.h"
#include <string.h>
// TODO Phase 5: Implement ConfigurationManager
// Use ESP32 NVS (Preferences library) to persist hardware profile + network settings.

static const RocConfig DEFAULT_CONFIG = {
    "ROCBoard",     // ssid
    "rocboard123",  // wifiPassword
    "ROCBoard-AP",  // apName
    1,              // ledCount
    48,             // ledPin
    50,             // brightness
};

ConfigurationManager::ConfigurationManager()
    : _config(DEFAULT_CONFIG)
    , _loaded(false)
{}

void ConfigurationManager::load() {
    _config = DEFAULT_CONFIG;
    _loaded = true;
    // TODO Phase 5: open Preferences namespace "roc", read each field, fall back to defaults
}

void ConfigurationManager::save() {
    // TODO Phase 5: open Preferences namespace "roc" in RW mode, write each field
}

const RocConfig& ConfigurationManager::config() const {
    return _config;
}

void ConfigurationManager::reset() {
    _config = DEFAULT_CONFIG;
    // TODO Phase 5: clear Preferences namespace "roc"
}
