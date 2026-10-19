#pragma once
#include <stdint.h>
// IHeartbeatService — Phase 6 interface stub.
// Periodic device registration and liveness reporting to the ROC backend.
// Enables remote monitoring of board connectivity and health.

struct HeartbeatPayload {
    char     deviceId[37];   // UUID string
    char     firmwareVersion[16];
    uint32_t uptimeSeconds;
    int8_t   wifiRssi;
    uint8_t  brightness;
    bool     puck0Connected;
    bool     puck1Connected;
    uint32_t freeHeap;
};

class IHeartbeatService {
public:
    virtual ~IHeartbeatService() = default;

    // Set device ID (UUID, assigned on first boot and persisted to NVS).
    virtual void setDeviceId(const char* uuid) = 0;

    // Set the registration endpoint URL.
    virtual void setEndpointUrl(const char* url) = 0;

    // Set heartbeat interval in seconds (default: 30).
    virtual void setIntervalSeconds(uint16_t seconds) = 0;

    // Force an immediate heartbeat send.
    virtual void sendNow() = 0;

    // Last known server response (e.g. updated display config from server).
    // Returns true if the server returned a new state payload.
    virtual bool getLastServerResponse(char* buf, uint16_t len) = 0;

    // Poll heartbeat timer (called from loop).
    virtual void poll() = 0;
};
