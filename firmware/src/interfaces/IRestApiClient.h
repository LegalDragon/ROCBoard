#pragma once
#include <stdint.h>
// IRestApiClient — Phase 6 interface stub.
// Communication with the ROC Board backend (pickleball.community or custom).
// Implements the transport half: sending/receiving JSON payloads over HTTPS.

class IRestApiClient {
public:
    virtual ~IRestApiClient() = default;

    // Set the base URL of the ROC backend API.
    virtual void setBaseUrl(const char* url) = 0;

    // Set authentication token (Bearer or API key).
    virtual void setAuthToken(const char* token) = 0;

    // POST a JSON command and receive a JSON response.
    // Returns true on HTTP 2xx, false on error.
    // responseBuf receives the response body (null-terminated).
    virtual bool post(const char* path, const char* jsonBody,
                      char* responseBuf, uint16_t responseLen) = 0;

    // GET a resource and receive a JSON response.
    virtual bool get(const char* path, char* responseBuf, uint16_t responseLen) = 0;

    // Register a state-push callback: called when server pushes new display state.
    // The payload is a JSON string (same format as set_display command).
    virtual void setOnStatePush(void (*cb)(const char* json)) = 0;

    // Poll for server-sent events or WebSocket messages (called from loop).
    virtual void poll() = 0;
};
