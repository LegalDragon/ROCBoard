#pragma once
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// ScoreboardState — shared state owned by main.cpp, updated by CommandProcessor,
// rendered by DisplayEngine.
// Phase 3+: full display state for the ROC Board.

struct ScoreboardState {
    char     topName[11];    // Top team name: 10 chars + null (alpha positions 0-9)
    char     bottomName[11]; // Bottom team name: 10 chars + null (alpha positions 10-19)
    uint8_t  topScore;       // Top team score 0-99
    uint8_t  bottomScore;    // Bottom team score 0-99
    uint32_t topColor;       // Top team LED color: 0xRRGGBB
    uint32_t bottomColor;    // Bottom team LED color: 0xRRGGBB
    uint8_t  brightness;     // Global brightness 0-255
    bool     swapped;        // If true, top/bottom are visually swapped

    ScoreboardState() {
        memset(topName,    ' ', 10); topName[10]    = '\0';
        memset(bottomName, ' ', 10); bottomName[10] = '\0';
        topScore    = 0;
        bottomScore = 0;
        topColor    = 0xFF0000; // red
        bottomColor = 0x0000FF; // blue
        brightness  = 50;
        swapped     = false;
    }

    // Parse a "#RRGGBB" hex color string. Returns white on error.
    static uint32_t parseHexColor(const char* hex) {
        if (!hex || hex[0] != '#' || strlen(hex) < 7) return 0xFFFFFF;
        unsigned int r = 0, g = 0, b = 0;
        if (sscanf(hex + 1, "%02x%02x%02x", &r, &g, &b) != 3) return 0xFFFFFF;
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    }
};
