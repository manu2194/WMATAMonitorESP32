/**
 * Unit tests for train group selection logic
 * 
 * Tests the logic for selecting one train per group (direction)
 * These tests run natively on your computer without ESP32 hardware.
 * 
 * Run with: pio test -e native
 */

#include <unity.h>
#include <cstring>
#include <cstdio>

#define MAX_TRAINS 2
#define DEST_MAX_LEN 5
#define MIN_MAX_LEN 4
#define LINE_MAX_LEN 3

struct TrainPrediction {
    char destination[DEST_MAX_LEN];
    char minutes[MIN_MAX_LEN];
    char line[LINE_MAX_LEN];
};

/**
 * Simulate the group selection logic from wmata_client.cpp
 * 
 * Given an array of train data (destination, minutes, group),
 * select the first train from each group (max 2 trains total)
 */
int selectTrainsByGroup(
    const char* destinations[],
    const char* minutes[],
    const char* groups[],
    int inputCount,
    TrainPrediction output[]
) {
    bool seenGroup1 = false;
    bool seenGroup2 = false;
    int outputCount = 0;
    
    for (int i = 0; i < inputCount && outputCount < MAX_TRAINS; i++) {
        bool isGroup1 = (strcmp(groups[i], "1") == 0);
        bool isGroup2 = (strcmp(groups[i], "2") == 0);
        
        if (isGroup1 && seenGroup1) continue;
        if (isGroup2 && seenGroup2) continue;
        
        // Copy truncated destination
        strncpy(output[outputCount].destination, destinations[i], DEST_MAX_LEN - 1);
        output[outputCount].destination[DEST_MAX_LEN - 1] = '\0';
        
        // Copy minutes
        strncpy(output[outputCount].minutes, minutes[i], MIN_MAX_LEN - 1);
        output[outputCount].minutes[MIN_MAX_LEN - 1] = '\0';
        
        outputCount++;
        
        if (isGroup1) seenGroup1 = true;
        if (isGroup2) seenGroup2 = true;
    }
    
    return outputCount;
}

// ============================================================================
// Group Selection Tests
// ============================================================================

void test_selects_one_train_per_group() {
    // Simulate: Glenmont(1), Shady Grv(2), Glenmont(1), Shady Grv(2)
    const char* destinations[] = {"Glenmont", "Shady Grv", "Glenmont", "Shady Grv"};
    const char* minutes[] = {"1", "1", "6", "8"};
    const char* groups[] = {"1", "2", "1", "2"};
    
    TrainPrediction output[MAX_TRAINS];
    int count = selectTrainsByGroup(destinations, minutes, groups, 4, output);
    
    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL_STRING("Glen", output[0].destination);  // First Group 1
    TEST_ASSERT_EQUAL_STRING("1", output[0].minutes);
    TEST_ASSERT_EQUAL_STRING("Shad", output[1].destination);  // First Group 2
    TEST_ASSERT_EQUAL_STRING("1", output[1].minutes);
}

void test_skips_duplicate_group_trains() {
    // All Group 1 trains - should only select the first one
    const char* destinations[] = {"Glenmont", "Glenmont", "Glenmont"};
    const char* minutes[] = {"1", "6", "11"};
    const char* groups[] = {"1", "1", "1"};
    
    TrainPrediction output[MAX_TRAINS];
    int count = selectTrainsByGroup(destinations, minutes, groups, 3, output);
    
    TEST_ASSERT_EQUAL(1, count);
    TEST_ASSERT_EQUAL_STRING("Glen", output[0].destination);
    TEST_ASSERT_EQUAL_STRING("1", output[0].minutes);
}

void test_handles_single_group() {
    // Only Group 2 trains
    const char* destinations[] = {"Shady", "Shady"};
    const char* minutes[] = {"2", "8"};
    const char* groups[] = {"2", "2"};
    
    TrainPrediction output[MAX_TRAINS];
    int count = selectTrainsByGroup(destinations, minutes, groups, 2, output);
    
    TEST_ASSERT_EQUAL(1, count);
    TEST_ASSERT_EQUAL_STRING("Shad", output[0].destination);
    TEST_ASSERT_EQUAL_STRING("2", output[0].minutes);
}

void test_empty_input() {
    TrainPrediction output[MAX_TRAINS];
    int count = selectTrainsByGroup(nullptr, nullptr, nullptr, 0, output);
    
    TEST_ASSERT_EQUAL(0, count);
}

void test_group2_first_then_group1() {
    // Group 2 comes first in API response (unusual but possible)
    const char* destinations[] = {"Shady", "Glenmont"};
    const char* minutes[] = {"3", "5"};
    const char* groups[] = {"2", "1"};
    
    TrainPrediction output[MAX_TRAINS];
    int count = selectTrainsByGroup(destinations, minutes, groups, 2, output);
    
    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL_STRING("Shad", output[0].destination);  // Group 2 first
    TEST_ASSERT_EQUAL_STRING("Glen", output[1].destination);  // Group 1 second
}

void test_many_trains_only_returns_two() {
    // Many trains but should only return 2 (one per group)
    const char* destinations[] = {"A", "B", "C", "D", "E", "F"};
    const char* minutes[] = {"1", "2", "3", "4", "5", "6"};
    const char* groups[] = {"1", "2", "1", "2", "1", "2"};
    
    TrainPrediction output[MAX_TRAINS];
    int count = selectTrainsByGroup(destinations, minutes, groups, 6, output);
    
    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL_STRING("A", output[0].destination);  // First Group 1
    TEST_ASSERT_EQUAL_STRING("B", output[1].destination);  // First Group 2
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
    
    RUN_TEST(test_selects_one_train_per_group);
    RUN_TEST(test_skips_duplicate_group_trains);
    RUN_TEST(test_handles_single_group);
    RUN_TEST(test_empty_input);
    RUN_TEST(test_group2_first_then_group1);
    RUN_TEST(test_many_trains_only_returns_two);
    
    return UNITY_END();
}
