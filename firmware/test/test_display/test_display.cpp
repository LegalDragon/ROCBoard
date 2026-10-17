#include <unity.h>
// TODO Phase 2: Add DisplayEngine tests once Phase 2 is implemented.

void test_placeholder(void) {
    // Phase 2 will add: test that renderText correctly maps chars to LED indices
    TEST_ASSERT_TRUE(true);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_placeholder);
    return UNITY_END();
}
