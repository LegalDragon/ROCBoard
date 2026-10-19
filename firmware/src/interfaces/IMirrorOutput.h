#pragma once
#include <stdint.h>
// IMirrorOutput — Phase 6 interface stub.
// Secondary mirrored scoreboard output.
// Allows a second ROC Board (or a display controller) to mirror
// the primary board's display state in real time over Wi-Fi or wired link.

class IMirrorOutput {
public:
    virtual ~IMirrorOutput() = default;

    // Set the destination address (IP:port for UDP, or URL for HTTP push).
    virtual void setDestination(const char* address, uint16_t port) = 0;

    // Push the current display state to the secondary board.
    // jsonState: a set_display JSON string.
    virtual bool pushState(const char* jsonState) = 0;

    // Enable/disable automatic push on every state change.
    virtual void setAutoMirror(bool enable) = 0;

    // Register callback for receiving mirrored state (secondary board mode).
    // When the primary board pushes state, this fires on the secondary.
    virtual void setOnReceive(void (*cb)(const char* jsonState)) = 0;

    // Poll mirror transport (called from loop).
    virtual void poll() = 0;
};
