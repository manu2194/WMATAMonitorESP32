/**
 * Unit tests for time utilities
 * 
 * Tests the 12-hour time conversion logic from time_utils.cpp
 * These tests run natively on your computer without ESP32 hardware.
 * 
 * Run with: pio test -e native
 */

#include <unity.h>

/**
 * Helper function that mirrors the 12-hour conversion logic from TimeManager
 * 
 * @param hour24 The 24-hour format hour (0-23)
 * @returns The 12-hour format hour (1-12)
 */
int convertTo12Hour(int hour24) {
    if (hour24 == 0) {
        return 12;  // Midnight
    } else if (hour24 > 12) {
        return hour24 - 12;  // PM hours
    } else {
        return hour24;  // AM hours (1-12)
    }
}

/**
 * Helper to check if hour is PM
 * 
 * @param hour24 The 24-hour format hour (0-23)
 * @returns True if PM, false if AM
 */
bool isPM(int hour24) {
    return hour24 >= 12;
}

// ============================================================================
// 12-Hour Conversion Tests
// ============================================================================

void test_midnight_converts_to_12() {
    TEST_ASSERT_EQUAL(12, convertTo12Hour(0));
}

void test_midnight_is_am() {
    TEST_ASSERT_FALSE(isPM(0));
}

void test_noon_converts_to_12() {
    TEST_ASSERT_EQUAL(12, convertTo12Hour(12));
}

void test_noon_is_pm() {
    TEST_ASSERT_TRUE(isPM(12));
}

void test_1am_converts_to_1() {
    TEST_ASSERT_EQUAL(1, convertTo12Hour(1));
}

void test_1am_is_am() {
    TEST_ASSERT_FALSE(isPM(1));
}

void test_11am_converts_to_11() {
    TEST_ASSERT_EQUAL(11, convertTo12Hour(11));
}

void test_11am_is_am() {
    TEST_ASSERT_FALSE(isPM(11));
}

void test_1pm_converts_to_1() {
    TEST_ASSERT_EQUAL(1, convertTo12Hour(13));
}

void test_1pm_is_pm() {
    TEST_ASSERT_TRUE(isPM(13));
}

void test_11pm_converts_to_11() {
    TEST_ASSERT_EQUAL(11, convertTo12Hour(23));
}

void test_11pm_is_pm() {
    TEST_ASSERT_TRUE(isPM(23));
}

void test_3pm_converts_to_3() {
    TEST_ASSERT_EQUAL(3, convertTo12Hour(15));
}

// ============================================================================
// Edge Case Tests
// ============================================================================

void test_all_hours_in_valid_range() {
    for (int h = 0; h < 24; h++) {
        int result = convertTo12Hour(h);
        TEST_ASSERT_TRUE(result >= 1 && result <= 12);
    }
}

// ============================================================================
// Test Runner
// ============================================================================

void setUp(void) {
    // Called before each test
}

void tearDown(void) {
    // Called after each test
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    // 12-hour conversion tests
    RUN_TEST(test_midnight_converts_to_12);
    RUN_TEST(test_midnight_is_am);
    RUN_TEST(test_noon_converts_to_12);
    RUN_TEST(test_noon_is_pm);
    RUN_TEST(test_1am_converts_to_1);
    RUN_TEST(test_1am_is_am);
    RUN_TEST(test_11am_converts_to_11);
    RUN_TEST(test_11am_is_am);
    RUN_TEST(test_1pm_converts_to_1);
    RUN_TEST(test_1pm_is_pm);
    RUN_TEST(test_11pm_converts_to_11);
    RUN_TEST(test_11pm_is_pm);
    RUN_TEST(test_3pm_converts_to_3);
    
    // Edge cases
    RUN_TEST(test_all_hours_in_valid_range);
    
    return UNITY_END();
}
