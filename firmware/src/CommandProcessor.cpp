#include "CommandProcessor.h"
#include "DisplayEngine.h"
#include "ScoreboardState.h"
#include <ArduinoJson.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

CommandProcessor::CommandProcessor()
    : _display(nullptr)
    , _state(nullptr)
    , _onStateChanged(nullptr)
    , _onSelfTest(nullptr)
    , _initialized(false)
{}

void CommandProcessor::init(DisplayEngine* display, ScoreboardState* state) {
    _display     = display;
    _state       = state;
    _initialized = true;
}

void CommandProcessor::setOnStateChanged(void (*cb)(void)) {
    _onStateChanged = cb;
}

void CommandProcessor::setOnSelfTest(void (*cb)(void)) {
    _onSelfTest = cb;
}

void CommandProcessor::_notifyStateChanged() {
    if (_onStateChanged) _onStateChanged();
}

// ─── Helper: copy and sanitize player names ────────────────────────────────

// Build a 10-char padded name from up to 2 player name strings.
// "JOHN", "MIKE" → "JOHN MIKE " (10 chars, right-padded with spaces)
static void buildTeamName(const char* p1, const char* p2, char* out) {
    char buf[11];
    memset(buf, ' ', 10);
    buf[10] = '\0';

    int pos = 0;
    for (int i = 0; p1 && p1[i] && pos < 10; i++) {
        buf[pos++] = (char)toupper((unsigned char)p1[i]);
    }
    if (p2 && p2[0] && pos < 10) {
        buf[pos++] = ' ';
        for (int i = 0; p2[i] && pos < 10; i++) {
            buf[pos++] = (char)toupper((unsigned char)p2[i]);
        }
    }
    memcpy(out, buf, 11);
}

// ─── Response builder ──────────────────────────────────────────────────────

void CommandProcessor::_buildStatusResponse(char* buf, uint16_t len) {
    if (!_state) {
        snprintf(buf, len, "{\"status\":\"ok\",\"error\":\"no state\"}");
        return;
    }
    snprintf(buf, len,
        "{\"status\":\"ok\",\"topName\":\"%s\",\"bottomName\":\"%s\","
        "\"topScore\":%d,\"bottomScore\":%d,\"brightness\":%d,\"swapped\":%s}",
        _state->topName, _state->bottomName,
        _state->topScore, _state->bottomScore,
        _state->brightness,
        _state->swapped ? "true" : "false");
}

// ─── Main process ──────────────────────────────────────────────────────────

bool CommandProcessor::process(const char* json, char* responseBuf, uint16_t responseLen) {
    if (!json || !responseBuf || responseLen == 0) return false;

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, json);
    if (err) {
        snprintf(responseBuf, responseLen,
            "{\"status\":\"error\",\"error\":\"JSON parse: %s\"}", err.c_str());
        return false;
    }

    const char* cmd = doc["cmd"] | "";

    // ── ping ──────────────────────────────────────────────────────────────
    if (strcmp(cmd, "ping") == 0 || strcmp(cmd, "PING") == 0) {
        snprintf(responseBuf, responseLen, "{\"status\":\"ok\",\"response\":\"PONG\"}");
        return true;
    }

    // ── status ────────────────────────────────────────────────────────────
    if (strcmp(cmd, "status") == 0) {
        _buildStatusResponse(responseBuf, responseLen);
        return true;
    }

    if (!_state) {
        snprintf(responseBuf, responseLen, "{\"status\":\"error\",\"error\":\"not initialized\"}");
        return false;
    }

    // ── set_display ───────────────────────────────────────────────────────
    if (strcmp(cmd, "set_display") == 0) {
        JsonObject top    = doc["top"];
        JsonObject bottom = doc["bottom"];

        if (!top.isNull()) {
            const char* p1 = top["players"][0] | "";
            const char* p2 = top["players"][1] | "";
            buildTeamName(p1, p2, _state->topName);
            _state->topScore = (uint8_t)(int)top["score"];
            const char* colorStr = top["color"] | "";
            if (colorStr[0]) _state->topColor = ScoreboardState::parseHexColor(colorStr);
        }

        if (!bottom.isNull()) {
            const char* p1 = bottom["players"][0] | "";
            const char* p2 = bottom["players"][1] | "";
            buildTeamName(p1, p2, _state->bottomName);
            _state->bottomScore = (uint8_t)(int)bottom["score"];
            const char* colorStr = bottom["color"] | "";
            if (colorStr[0]) _state->bottomColor = ScoreboardState::parseHexColor(colorStr);
        }

        _notifyStateChanged();
        snprintf(responseBuf, responseLen, "{\"status\":\"ok\"}");
        return true;
    }

    // ── swap_teams ────────────────────────────────────────────────────────
    if (strcmp(cmd, "swap_teams") == 0) {
        _state->swapped = !_state->swapped;
        _notifyStateChanged();
        snprintf(responseBuf, responseLen,
            "{\"status\":\"ok\",\"swapped\":%s}",
            _state->swapped ? "true" : "false");
        return true;
    }

    // ── brightness ────────────────────────────────────────────────────────
    if (strcmp(cmd, "brightness") == 0) {
        int val = doc["value"] | -1;
        if (val < 0 || val > 255) {
            snprintf(responseBuf, responseLen,
                "{\"status\":\"error\",\"error\":\"value must be 0-255\"}");
            return false;
        }
        _state->brightness = (uint8_t)val;
        _notifyStateChanged();
        snprintf(responseBuf, responseLen,
            "{\"status\":\"ok\",\"brightness\":%d}", val);
        return true;
    }

    // ── self_test ─────────────────────────────────────────────────────────
    if (strcmp(cmd, "self_test") == 0) {
        if (_onSelfTest) _onSelfTest();
        snprintf(responseBuf, responseLen, "{\"status\":\"ok\",\"test\":\"running\"}");
        return true;
    }

    // ── clear ─────────────────────────────────────────────────────────────
    if (strcmp(cmd, "clear") == 0 || strcmp(cmd, "CLEAR") == 0) {
        memset(_state->topName,    ' ', 10); _state->topName[10]    = '\0';
        memset(_state->bottomName, ' ', 10); _state->bottomName[10] = '\0';
        _state->topScore    = 0;
        _state->bottomScore = 0;
        _notifyStateChanged();
        snprintf(responseBuf, responseLen, "{\"status\":\"ok\"}");
        return true;
    }

    // ── SET_SCORE (legacy compat) ──────────────────────────────────────────
    if (strcmp(cmd, "SET_SCORE") == 0) {
        int scoreA = doc["teamA"] | (int)_state->topScore;
        int scoreB = doc["teamB"] | (int)_state->bottomScore;
        _state->topScore    = (uint8_t)scoreA;
        _state->bottomScore = (uint8_t)scoreB;
        _notifyStateChanged();
        snprintf(responseBuf, responseLen, "{\"status\":\"ok\"}");
        return true;
    }

    // ── SET_NAME (legacy compat) ───────────────────────────────────────────
    if (strcmp(cmd, "SET_NAME") == 0) {
        const char* team = doc["team"] | "";
        const char* name = doc["name"] | "";
        if (strcmp(team, "A") == 0 || strcmp(team, "top") == 0) {
            buildTeamName(name, "", _state->topName);
        } else if (strcmp(team, "B") == 0 || strcmp(team, "bottom") == 0) {
            buildTeamName(name, "", _state->bottomName);
        } else {
            snprintf(responseBuf, responseLen,
                "{\"status\":\"error\",\"error\":\"unknown team\"}");
            return false;
        }
        _notifyStateChanged();
        snprintf(responseBuf, responseLen, "{\"status\":\"ok\"}");
        return true;
    }

    snprintf(responseBuf, responseLen,
        "{\"status\":\"error\",\"error\":\"unknown cmd: %s\"}", cmd);
    return false;
}
