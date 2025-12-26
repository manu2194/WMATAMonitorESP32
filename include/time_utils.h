#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <Arduino.h>

/**
 * Structure to hold formatted time data
 */
struct TimeData {
    int hour;      // 12-hour format (1-12)
    int minute;    // 0-59
    int second;    // 0-59
    bool isPM;     // True for PM
    bool isValid;  // True if time was retrieved successfully
};

/**
 * TimeManager class to handle NTP sync and time formatting
 */
class TimeManager {
public:
    TimeManager();
    
    /**
     * Synchronize time with NTP servers
     */
    void syncNTP();
    
    /**
     * Get the current time formatted for display
     * 
     * :return TimeData: Structure containing formatted time components
     */
    TimeData getCurrentTime();
    
    /**
     * Get formatted time string in HH:MM:SS format
     * 
     * :param char* buffer: Buffer to store the formatted string (min 9 chars)
     * :param size_t bufferSize: Size of the buffer
     * :return bool: True if time was formatted successfully
     */
    bool getFormattedTime(char* buffer, size_t bufferSize);
};

#endif // TIME_UTILS_H
