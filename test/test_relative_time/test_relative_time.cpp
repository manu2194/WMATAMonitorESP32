/**
 * Unit tests for relative time formatting
 * 
 * Tests the formatRelativeTime and getRelativeTimeComponents functions
 * These tests run natively on your computer without ESP32 hardware.
 * 
 * Run with: pio test -e native
 */

#include <unity.h>
#include <cstring>
#include <cstdio>

// Include the header (we'll need to replicate the declarations for native testing)
// In a real setup, you'd have proper include paths set up

/**
 * Pure C++ implementation of the relative time functions for testing
 * (These mirror the actual implementation in relative_time.cpp)
 */
void getRelativeTimeComponents(unsigned long elapsedMs, int& value, char& unit) {
    unsigned long elapsedSec = elapsedMs / 1000;
    
    if (elapsedSec < 90) {
        value = (int)elapsedSec;
        unit = 's';
        if (value > 99) {
            value = 99;
        }
    } else {
        unsigned long elapsedMin = elapsedSec / 60;
        value = (int)elapsedMin;
        unit = 'm';
        if (value > 99) {
            value = 99;
        }
    }
}

void formatRelativeTime(unsigned long elapsedMs, char* buffer, size_t bufferSize) {
    if (buffer == nullptr || bufferSize < 2) {
        return;
    }
    
    int value;
    char unit;
    getRelativeTimeComponents(elapsedMs, value, unit);
    
    snprintf(buffer, bufferSize, "%d %c ago", value, unit);
}

// ============================================================================
// Seconds Tests (0-89 seconds)
// ============================================================================

void test_zero_seconds() {
    int value;
    char unit;
    getRelativeTimeComponents(0, value, unit);
    
    TEST_ASSERT_EQUAL(0, value);
    TEST_ASSERT_EQUAL('s', unit);
}

void test_one_second() {
    int value;
    char unit;
    getRelativeTimeComponents(1000, value, unit);
    
    TEST_ASSERT_EQUAL(1, value);
    TEST_ASSERT_EQUAL('s', unit);
}

void test_59_seconds() {
    int value;
    char unit;
    getRelativeTimeComponents(59000, value, unit);
    
    TEST_ASSERT_EQUAL(59, value);
    TEST_ASSERT_EQUAL('s', unit);
}

void test_89_seconds() {
    int value;
    char unit;
    getRelativeTimeComponents(89000, value, unit);
    
    TEST_ASSERT_EQUAL(89, value);
    TEST_ASSERT_EQUAL('s', unit);
}

// ============================================================================
// Minutes Tests (90+ seconds)
// ============================================================================

void test_90_seconds_shows_minutes() {
    int value;
    char unit;
    getRelativeTimeComponents(90000, value, unit);  // 90 seconds = 1.5 minutes
    
    TEST_ASSERT_EQUAL(1, value);
    TEST_ASSERT_EQUAL('m', unit);
}

void test_120_seconds_shows_2_minutes() {
    int value;
    char unit;
    getRelativeTimeComponents(120000, value, unit);  // 120 seconds = 2 minutes
    
    TEST_ASSERT_EQUAL(2, value);
    TEST_ASSERT_EQUAL('m', unit);
}

void test_5_minutes() {
    int value;
    char unit;
    getRelativeTimeComponents(300000, value, unit);  // 5 minutes
    
    TEST_ASSERT_EQUAL(5, value);
    TEST_ASSERT_EQUAL('m', unit);
}

void test_15_minutes() {
    int value;
    char unit;
    getRelativeTimeComponents(900000, value, unit);  // 15 minutes
    
    TEST_ASSERT_EQUAL(15, value);
    TEST_ASSERT_EQUAL('m', unit);
}

// ============================================================================
// Cap at 99 Tests
// ============================================================================

void test_99_minutes() {
    int value;
    char unit;
    getRelativeTimeComponents(99 * 60 * 1000, value, unit);
    
    TEST_ASSERT_EQUAL(99, value);
    TEST_ASSERT_EQUAL('m', unit);
}

void test_100_minutes_caps_at_99() {
    int value;
    char unit;
    getRelativeTimeComponents(100 * 60 * 1000, value, unit);
    
    TEST_ASSERT_EQUAL(99, value);
    TEST_ASSERT_EQUAL('m', unit);
}

void test_1000_minutes_caps_at_99() {
    int value;
    char unit;
    getRelativeTimeComponents(1000 * 60 * 1000UL, value, unit);
    
    TEST_ASSERT_EQUAL(99, value);
    TEST_ASSERT_EQUAL('m', unit);
}

// ============================================================================
// Format String Tests
// ============================================================================

void test_format_1_second() {
    char buffer[16];
    formatRelativeTime(1000, buffer, sizeof(buffer));
    
    TEST_ASSERT_EQUAL_STRING("1 s ago", buffer);
}

void test_format_89_seconds() {
    char buffer[16];
    formatRelativeTime(89000, buffer, sizeof(buffer));
    
    TEST_ASSERT_EQUAL_STRING("89 s ago", buffer);
}

void test_format_2_minutes() {
    char buffer[16];
    formatRelativeTime(120000, buffer, sizeof(buffer));
    
    TEST_ASSERT_EQUAL_STRING("2 m ago", buffer);
}

void test_format_15_minutes() {
    char buffer[16];
    formatRelativeTime(900000, buffer, sizeof(buffer));
    
    TEST_ASSERT_EQUAL_STRING("15 m ago", buffer);
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
    
    // Seconds tests
    RUN_TEST(test_zero_seconds);
    RUN_TEST(test_one_second);
    RUN_TEST(test_59_seconds);
    RUN_TEST(test_89_seconds);
    
    // Minutes tests
    RUN_TEST(test_90_seconds_shows_minutes);
    RUN_TEST(test_120_seconds_shows_2_minutes);
    RUN_TEST(test_5_minutes);
    RUN_TEST(test_15_minutes);
    
    // Cap tests
    RUN_TEST(test_99_minutes);
    RUN_TEST(test_100_minutes_caps_at_99);
    RUN_TEST(test_1000_minutes_caps_at_99);
    
    // Format string tests
    RUN_TEST(test_format_1_second);
    RUN_TEST(test_format_89_seconds);
    RUN_TEST(test_format_2_minutes);
    RUN_TEST(test_format_15_minutes);
    
    return UNITY_END();
}
