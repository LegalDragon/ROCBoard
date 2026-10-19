#pragma once
#include <stdint.h>
// IBluetoothPuck — Phase 6 interface stub.
// BLE connection to ROC scoring pucks (up to 2 simultaneous).
// Each puck sends score events (point scored, fault, etc.) via BLE notifications.
// The ROC Board is the BLE Central; pucks are Peripherals.

// A single score event from a puck.
struct PuckEvent {
    uint8_t puckId;      // 0 or 1
    uint8_t eventType;   // 0=point, 1=fault, 2=timeout, 3=end_game
    uint8_t team;        // 0=top, 1=bottom (puck assignment)
};

class IBluetoothPuck {
public:
    virtual ~IBluetoothPuck() = default;

    // Start scanning for ROC pucks.
    virtual void startScan() = 0;

    // Stop scanning.
    virtual void stopScan() = 0;

    // Connect to a specific puck by MAC address.
    virtual bool connectPuck(uint8_t puckId, const char* macAddress) = 0;

    // Disconnect a puck.
    virtual void disconnectPuck(uint8_t puckId) = 0;

    // Returns true if a specific puck is connected.
    virtual bool isPuckConnected(uint8_t puckId) const = 0;

    // Register callback for incoming puck events.
    virtual void setOnPuckEvent(void (*cb)(const PuckEvent& event)) = 0;

    // Send OTA firmware update to a puck.
    // TODO: Phase 6+ — relay firmware binary to puck over BLE OTA.
    virtual bool sendOtaToPuck(uint8_t puckId,
                               const uint8_t* firmwareBuf, uint32_t firmwareLen) = 0;

    // Poll BLE stack (called from loop).
    virtual void poll() = 0;
};
