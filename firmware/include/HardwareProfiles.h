#pragma once
#include <stdint.h>

// ROCBoard Hardware Profiles
// Each profile targets a specific LED configuration for the scoreboard.

// Profile ID constants — used with HARDWARE_PROFILE build flag.
// Note: names intentionally differ from the HardwareProfile struct constants
// below (which use no underscores) to avoid preprocessor collisions.
#define PROFILE_1_LED    1   // Phase 1: onboard RGB, GPIO48
#define PROFILE_14_LED   2   // Phase 2: single 14-seg char, GPIO1, 14 LEDs
#define PROFILE_28_LED   3   // Phase 3: two characters, GPIO1, 28 LEDs
#define PROFILE_FULL_ID  4   // Final board: 336 LEDs, multiple GPIOs

struct HardwareProfile {
    uint8_t  ledPin;
    uint16_t ledCount;
    const char* name;
};

// Phase 1: Single onboard WS2812B LED (GPIO48 on ESP32-S3-DevKitC-1 N8R8)
static constexpr HardwareProfile PROFILE_1LED = {
    .ledPin   = 48,
    .ledCount = 1,
    .name     = "1LED_TEST"
};

// Phase 2: 14 LEDs — one 14-segment character
static constexpr HardwareProfile PROFILE_14LED = {
    .ledPin   = 1,
    .ledCount = 14,
    .name     = "14LED_SINGLE_CHAR"
};

// Phase 3: 28 LEDs — two 14-segment characters
static constexpr HardwareProfile PROFILE_28LED = {
    .ledPin   = 1,
    .ledCount = 28,
    .name     = "28LED_TWO_CHAR"
};

// Phase 4+: Full scoreboard — 20 alpha (14-seg) + 4 numeric (7-seg) characters
// 20 chars × 14 segments + 4 chars × 7 segments = 308 LEDs
static constexpr HardwareProfile PROFILE_FULL = {
    .ledPin   = 48,
    .ledCount = 308,
    .name     = "FULL_SCOREBOARD"
};
