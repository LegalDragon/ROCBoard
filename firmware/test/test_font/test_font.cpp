#include <unity.h>
#include "../../src/Font14Segment.h"

using namespace Font14Seg;

void test_A_segments(void) {
    SegmentMask mask = charToMask('A');
    // A should have: a, b, c, e, f, g, h (middle bar = g+h, all sides + top)
    TEST_ASSERT_TRUE(mask & SEG_A); // top
    TEST_ASSERT_TRUE(mask & SEG_B); // upper right
    TEST_ASSERT_TRUE(mask & SEG_C); // lower right
    TEST_ASSERT_TRUE(mask & SEG_E); // lower left
    TEST_ASSERT_TRUE(mask & SEG_F); // upper left
    TEST_ASSERT_TRUE(mask & SEG_G); // middle left
    TEST_ASSERT_TRUE(mask & SEG_H); // middle right
    TEST_ASSERT_FALSE(mask & SEG_D); // no bottom
    TEST_ASSERT_FALSE(mask & SEG_DP); // no decimal point
}

void test_zero_segments(void) {
    SegmentMask mask = charToMask('0');
    // 0 should have: a, b, c, d, e, f (all outer segments, no middle)
    TEST_ASSERT_TRUE(mask & SEG_A);
    TEST_ASSERT_TRUE(mask & SEG_B);
    TEST_ASSERT_TRUE(mask & SEG_C);
    TEST_ASSERT_TRUE(mask & SEG_D);
    TEST_ASSERT_TRUE(mask & SEG_E);
    TEST_ASSERT_TRUE(mask & SEG_F);
    // No middle segments
    TEST_ASSERT_FALSE(mask & SEG_G);
    TEST_ASSERT_FALSE(mask & SEG_H);
    TEST_ASSERT_FALSE(mask & SEG_DP);
}

void test_space_is_blank(void) {
    SegmentMask mask = charToMask(' ');
    TEST_ASSERT_EQUAL(0, mask);
}

void test_lowercase_mapped_to_uppercase(void) {
    TEST_ASSERT_EQUAL(charToMask('A'), charToMask('a'));
    TEST_ASSERT_EQUAL(charToMask('Z'), charToMask('z'));
}

void test_unknown_char_is_blank(void) {
    // Characters outside normal range should return 0
    TEST_ASSERT_EQUAL(0, charToMask('\x01'));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_A_segments);
    RUN_TEST(test_zero_segments);
    RUN_TEST(test_space_is_blank);
    RUN_TEST(test_lowercase_mapped_to_uppercase);
    RUN_TEST(test_unknown_char_is_blank);
    return UNITY_END();
}
