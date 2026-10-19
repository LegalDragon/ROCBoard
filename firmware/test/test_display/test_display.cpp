#include <unity.h>
#include "../../src/LayoutMapper.h"
#include "../../src/LayoutMapper.cpp"
#include "../../src/ScoreboardState.h"

// ─── LayoutMapper tests ────────────────────────────────────────────────────

void test_layout_phase2_char0_seg0(void) {
    // Phase 2: charPos=0, seg=0 → LED 0
    uint16_t idx = LayoutMapper::getPhysicalIndex(0, 0);
    TEST_ASSERT_EQUAL(0, idx);
}

void test_layout_phase2_char0_seg13(void) {
    // Phase 2: charPos=0, seg=13 → LED 13
    uint16_t idx = LayoutMapper::getPhysicalIndex(0, 13);
    TEST_ASSERT_EQUAL(13, idx);
}

void test_layout_phase2_char1_seg0(void) {
    // charPos=1 maps to base 14 in FULL, invalid in 14LED
    // In NATIVE_TEST / default mode, full mapping is used:
    uint16_t idx = LayoutMapper::getPhysicalIndex(1, 0);
    TEST_ASSERT_EQUAL(14, idx);  // pos 1, seg 0 = 1*14+0 = 14
}

void test_layout_full_pos10_seg0(void) {
    // Full board: charPos=10, seg=0 → LED 10*14+0 = 140
    uint16_t idx = LayoutMapper::getPhysicalIndex(10, 0);
    TEST_ASSERT_EQUAL(140, idx);
}

void test_layout_full_pos23_seg13(void) {
    // Full board: last LED = charPos=23, seg=13 → 23*14+13 = 335
    uint16_t idx = LayoutMapper::getPhysicalIndex(23, 13);
    TEST_ASSERT_EQUAL(335, idx);
}

void test_layout_invalid_seg(void) {
    // segment 14 → INVALID
    uint16_t idx = LayoutMapper::getPhysicalIndex(0, 14);
    TEST_ASSERT_EQUAL(INVALID_LED_INDEX, idx);
}

void test_layout_invalid_charpos(void) {
    // charPos 24 → INVALID
    uint16_t idx = LayoutMapper::getPhysicalIndex(24, 0);
    TEST_ASSERT_EQUAL(INVALID_LED_INDEX, idx);
}

// ─── ScoreboardState tests ─────────────────────────────────────────────────

void test_state_defaults(void) {
    ScoreboardState state;
    TEST_ASSERT_EQUAL(0, state.topScore);
    TEST_ASSERT_EQUAL(0, state.bottomScore);
    TEST_ASSERT_EQUAL(50, state.brightness);
    TEST_ASSERT_FALSE(state.swapped);
}

void test_state_parse_hex_color(void) {
    uint32_t color = ScoreboardState::parseHexColor("#FF0000");
    TEST_ASSERT_EQUAL(0xFF0000u, color);

    color = ScoreboardState::parseHexColor("#0000FF");
    TEST_ASSERT_EQUAL(0x0000FFu, color);

    color = ScoreboardState::parseHexColor("#1A2B3C");
    TEST_ASSERT_EQUAL(0x1A2B3Cu, color);
}

void test_state_parse_hex_color_invalid(void) {
    // Invalid → white
    uint32_t color = ScoreboardState::parseHexColor(nullptr);
    TEST_ASSERT_EQUAL(0xFFFFFFu, color);

    color = ScoreboardState::parseHexColor("bad");
    TEST_ASSERT_EQUAL(0xFFFFFFu, color);
}

// ─── CommandProcessor tests ────────────────────────────────────────────────

#include "../../src/CommandProcessor.h"
#include "../../src/CommandProcessor.cpp"
#include "../../src/DisplayEngine.h"
#include "../../src/DisplayEngine.cpp"
// Include all deps for native compilation (LayoutMapper already included above)
#include "../../src/Font14Segment.cpp"
#include "../../src/LedDriver.cpp"

void test_cmd_ping(void) {
    ScoreboardState state;
    CommandProcessor cp;
    cp.init(nullptr, &state);

    char resp[128];
    bool ok = cp.process("{\"cmd\":\"ping\"}", resp, sizeof(resp));
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_NOT_NULL(strstr(resp, "PONG"));
}

void test_cmd_set_display(void) {
    ScoreboardState state;
    CommandProcessor cp;
    cp.init(nullptr, &state);

    char resp[128];
    bool ok = cp.process(
        "{\"cmd\":\"set_display\","
        "\"top\":{\"players\":[\"JOHN\",\"MIKE\"],\"score\":8,\"color\":\"#FF0000\"},"
        "\"bottom\":{\"players\":[\"DAVID\",\"PETER\"],\"score\":6,\"color\":\"#0000FF\"}}",
        resp, sizeof(resp));

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL(8, state.topScore);
    TEST_ASSERT_EQUAL(6, state.bottomScore);
    TEST_ASSERT_EQUAL(0xFF0000u, state.topColor);
    TEST_ASSERT_EQUAL(0x0000FFu, state.bottomColor);
    // Name should start with "JOHN MIKE"
    TEST_ASSERT_EQUAL('J', state.topName[0]);
    TEST_ASSERT_EQUAL('O', state.topName[1]);
}

void test_cmd_swap_teams(void) {
    ScoreboardState state;
    CommandProcessor cp;
    cp.init(nullptr, &state);

    char resp[128];
    cp.process("{\"cmd\":\"swap_teams\"}", resp, sizeof(resp));
    TEST_ASSERT_TRUE(state.swapped);
    cp.process("{\"cmd\":\"swap_teams\"}", resp, sizeof(resp));
    TEST_ASSERT_FALSE(state.swapped);
}

void test_cmd_brightness(void) {
    ScoreboardState state;
    CommandProcessor cp;
    cp.init(nullptr, &state);

    char resp[128];
    bool ok = cp.process("{\"cmd\":\"brightness\",\"value\":200}", resp, sizeof(resp));
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL(200, state.brightness);
}

void test_cmd_brightness_invalid(void) {
    ScoreboardState state;
    CommandProcessor cp;
    cp.init(nullptr, &state);
    state.brightness = 50;

    char resp[128];
    bool ok = cp.process("{\"cmd\":\"brightness\",\"value\":300}", resp, sizeof(resp));
    TEST_ASSERT_FALSE(ok); // 300 > 255, should fail
    TEST_ASSERT_EQUAL(50, state.brightness); // unchanged
}

void test_cmd_clear(void) {
    ScoreboardState state;
    state.topScore = 9;
    state.bottomScore = 7;
    CommandProcessor cp;
    cp.init(nullptr, &state);

    char resp[128];
    cp.process("{\"cmd\":\"clear\"}", resp, sizeof(resp));
    TEST_ASSERT_EQUAL(0, state.topScore);
    TEST_ASSERT_EQUAL(0, state.bottomScore);
}

void test_cmd_unknown(void) {
    ScoreboardState state;
    CommandProcessor cp;
    cp.init(nullptr, &state);

    char resp[128];
    bool ok = cp.process("{\"cmd\":\"nonexistent\"}", resp, sizeof(resp));
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_NOT_NULL(strstr(resp, "error"));
}

// ─── Runner ────────────────────────────────────────────────────────────────

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_layout_phase2_char0_seg0);
    RUN_TEST(test_layout_phase2_char0_seg13);
    RUN_TEST(test_layout_phase2_char1_seg0);
    RUN_TEST(test_layout_full_pos10_seg0);
    RUN_TEST(test_layout_full_pos23_seg13);
    RUN_TEST(test_layout_invalid_seg);
    RUN_TEST(test_layout_invalid_charpos);

    RUN_TEST(test_state_defaults);
    RUN_TEST(test_state_parse_hex_color);
    RUN_TEST(test_state_parse_hex_color_invalid);

    RUN_TEST(test_cmd_ping);
    RUN_TEST(test_cmd_set_display);
    RUN_TEST(test_cmd_swap_teams);
    RUN_TEST(test_cmd_brightness);
    RUN_TEST(test_cmd_brightness_invalid);
    RUN_TEST(test_cmd_clear);
    RUN_TEST(test_cmd_unknown);

    return UNITY_END();
}
