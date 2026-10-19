#pragma once
// IWifiTransport — Phase 6 interface stub.
// Provides Wi-Fi connectivity management for the ROC Board.
// Implementation: will use ESP32 WiFi library (STA for network join, AP for setup portal).

class IWifiTransport {
public:
    virtual ~IWifiTransport() = default;

    // Connect to a Wi-Fi network in STA mode.
    virtual bool connect(const char* ssid, const char* password) = 0;

    // Start a Wi-Fi access point for initial configuration.
    virtual bool startAP(const char* ssid, const char* password) = 0;

    // Disconnect and stop all Wi-Fi.
    virtual void disconnect() = 0;

    // Returns true if connected to a network.
    virtual bool isConnected() const = 0;

    // Returns current RSSI (signal strength) or 0 if not connected.
    virtual int8_t getRSSI() const = 0;

    // Poll for incoming REST/WebSocket data (called from loop).
    virtual void poll() = 0;
};
