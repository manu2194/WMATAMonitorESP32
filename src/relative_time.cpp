#include "relative_time.h"
#include <stdio.h>

void getRelativeTimeComponents(unsigned long elapsedMs, int& value, char& unit) {
    unsigned long elapsedSec = elapsedMs / 1000;
    
    // If less than 90 seconds, show in seconds
    if (elapsedSec < 90) {
        value = (int)elapsedSec;
        unit = 's';
        
        // Cap at 99 (should not happen in seconds mode, but safety check)
        if (value > 99) {
            value = 99;
        }
    } else {
        // Convert to minutes
        unsigned long elapsedMin = elapsedSec / 60;
        value = (int)elapsedMin;
        unit = 'm';
        
        // Cap at 99 minutes
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
    
    // Format: "X s ago" or "XX m ago"
    snprintf(buffer, bufferSize, "%d %c ago", value, unit);
}
