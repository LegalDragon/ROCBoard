#pragma once
#include <stdint.h>
// IOtaManager — Phase 6 interface stub.
// Over-the-air firmware update management for the ROC Board itself.
// Implementation will use ESP32 Update library + HTTPS firmware fetch.

enum class OtaState {
    IDLE,
    CHECKING,
    DOWNLOADING,
    APPLYING,
    REBOOT_PENDING,
    ERROR,
};

class IOtaManager {
public:
    virtual ~IOtaManager() = default;

    // Set the URL where firmware manifest/binary lives.
    virtual void setFirmwareUrl(const char* url) = 0;

    // Check for a newer version. Returns true if update available.
    virtual bool checkForUpdate() = 0;

    // Start downloading + applying the update. Non-blocking; poll getState().
    virtual void startUpdate() = 0;

    // Cancel an in-progress update.
    virtual void cancelUpdate() = 0;

    // Current OTA state machine state.
    virtual OtaState getState() const = 0;

    // Download progress 0-100.
    virtual uint8_t getProgress() const = 0;

    // Register callback for OTA completion (reboots if success).
    virtual void setOnComplete(void (*cb)(bool success)) = 0;

    // Poll OTA state machine (called from loop).
    virtual void poll() = 0;
};
