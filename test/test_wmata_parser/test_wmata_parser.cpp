/**
 * Unit tests for WMATA JSON parsing
 * 
 * Tests the logic for parsing WMATA API responses and extracting train data.
 * These tests run natively on your computer without ESP32 hardware.
 * 
 * Run with: pio test -e native
 */

#include <unity.h>
#include <cstring>

/**
 * Simple struct mirroring TrainPrediction for testing
 * Destination is now limited to 4 letters for LED display
 */
#define DEST_MAX_LEN 5
#define MIN_MAX_LEN 4
#define LINE_MAX_LEN 3

struct TrainPrediction {
    char destination[DEST_MAX_LEN];
    char minutes[MIN_MAX_LEN];
    char line[LINE_MAX_LEN];
};

/**
 * Parse train data into a TrainPrediction struct
 * (Mirrors the logic in WmataClient::_parseTrainObject)
 */
void parseTrainObject(const char* destination, const char* minutes, const char* line, TrainPrediction& prediction) {
    strncpy(prediction.destination, destination, DEST_MAX_LEN - 1);
    prediction.destination[DEST_MAX_LEN - 1] = '\0';
    
    strncpy(prediction.minutes, minutes, MIN_MAX_LEN - 1);
    prediction.minutes[MIN_MAX_LEN - 1] = '\0';
    
    strncpy(prediction.line, line, LINE_MAX_LEN - 1);
    prediction.line[LINE_MAX_LEN - 1] = '\0';
}

// ============================================================================
// Destination Parsing Tests
// ============================================================================

void test_short_destination() {
    TrainPrediction train;
    parseTrainObject("Glenmont", "5", "RD", train);
    
    // Truncated to first 4 letters
    TEST_ASSERT_EQUAL_STRING("Glen", train.destination);
}

void test_long_destination_truncated() {
    TrainPrediction train;
    parseTrainObject("Shady Grove", "3", "RD", train);
    
    // Truncated to first 4 letters
    TEST_ASSERT_EQUAL_STRING("Shad", train.destination);
}

void test_destination_with_special_chars() {
    TrainPrediction train;
    parseTrainObject("Shady Grv", "1", "RD", train);
    
    // Truncated to first 4 letters
    TEST_ASSERT_EQUAL_STRING("Shad", train.destination);
}

// ============================================================================
// Minutes Parsing Tests
// ============================================================================

void test_single_digit_minutes() {
    TrainPrediction train;
    parseTrainObject("Tst", "5", "RD", train);
    
    TEST_ASSERT_EQUAL_STRING("5", train.minutes);
}

void test_double_digit_minutes() {
    TrainPrediction train;
    parseTrainObject("Tst", "12", "RD", train);
    
    TEST_ASSERT_EQUAL_STRING("12", train.minutes);
}

void test_arr_minutes() {
    TrainPrediction train;
    parseTrainObject("Tst", "ARR", "RD", train);
    
    TEST_ASSERT_EQUAL_STRING("ARR", train.minutes);
}

void test_brd_minutes() {
    TrainPrediction train;
    parseTrainObject("Tst", "BRD", "RD", train);
    
    TEST_ASSERT_EQUAL_STRING("BRD", train.minutes);
}

// ============================================================================
// Line Code Tests
// ============================================================================

void test_red_line() {
    TrainPrediction train;
    parseTrainObject("Tst", "5", "RD", train);
    
    TEST_ASSERT_EQUAL_STRING("RD", train.line);
}

void test_blue_line() {
    TrainPrediction train;
    parseTrainObject("Tst", "5", "BL", train);
    
    TEST_ASSERT_EQUAL_STRING("BL", train.line);
}

void test_orange_line() {
    TrainPrediction train;
    parseTrainObject("Tst", "5", "OR", train);
    
    TEST_ASSERT_EQUAL_STRING("OR", train.line);
}

void test_green_line() {
    TrainPrediction train;
    parseTrainObject("Tst", "5", "GR", train);
    
    TEST_ASSERT_EQUAL_STRING("GR", train.line);
}

void test_yellow_line() {
    TrainPrediction train;
    parseTrainObject("Tst", "5", "YL", train);
    
    TEST_ASSERT_EQUAL_STRING("YL", train.line);
}

void test_silver_line() {
    TrainPrediction train;
    parseTrainObject("Tst", "5", "SV", train);
    
    TEST_ASSERT_EQUAL_STRING("SV", train.line);
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_empty_destination() {
    TrainPrediction train;
    parseTrainObject("", "5", "RD", train);
    
    TEST_ASSERT_EQUAL_STRING("", train.destination);
}

void test_empty_minutes() {
    TrainPrediction train;
    parseTrainObject("Test", "", "RD", train);
    
    TEST_ASSERT_EQUAL_STRING("", train.minutes);
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
    
    // Destination tests
    RUN_TEST(test_short_destination);
    RUN_TEST(test_long_destination_truncated);
    RUN_TEST(test_destination_with_special_chars);
    
    // Minutes tests
    RUN_TEST(test_single_digit_minutes);
    RUN_TEST(test_double_digit_minutes);
    RUN_TEST(test_arr_minutes);
    RUN_TEST(test_brd_minutes);
    
    // Line code tests
    RUN_TEST(test_red_line);
    RUN_TEST(test_blue_line);
    RUN_TEST(test_orange_line);
    RUN_TEST(test_green_line);
    RUN_TEST(test_yellow_line);
    RUN_TEST(test_silver_line);
    
    // Edge cases
    RUN_TEST(test_empty_destination);
    RUN_TEST(test_empty_minutes);
    
    return UNITY_END();
}
