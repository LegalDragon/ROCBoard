#include "ConfigurationManager.h"
#include <string.h>
// TODO Phase 5: Implement ConfigurationManager
// Use ESP32 NVS (Preferences library) to persist hardware profile + network settings.

static const RocConfig DEFAULT_CONFIG = {
    .ssid         = "ROCBoard",
    .wifiPassword = "rocboard123",
    .apName       = "ROCBoard-AP",
    .ledCount     = 1,
    .ledPin       = 48,
    .brightness   = 50,
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
