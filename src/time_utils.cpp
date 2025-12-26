#include "time_utils.h"
#include "config.h"
#include <time.h>

TimeManager::TimeManager() {}

void TimeManager::syncNTP() {
    configTime(TIMEZONE_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
    Serial.println("NTP time sync initiated");
}

TimeData TimeManager::getCurrentTime() {
    TimeData data = {0, 0, 0, false, false};
    
    struct tm timeInfo;
    if (!getLocalTime(&timeInfo)) {
        return data;
    }
    
    data.isValid = true;
    
    // Convert to 12-hour format
    int hour24 = timeInfo.tm_hour;
    data.isPM = (hour24 >= 12);
    
    if (hour24 == 0) {
        data.hour = 12;
    } else if (hour24 > 12) {
        data.hour = hour24 - 12;
    } else {
        data.hour = hour24;
    }
    
    data.minute = timeInfo.tm_min;
    data.second = timeInfo.tm_sec;
    
    return data;
}

bool TimeManager::getFormattedTime(char* buffer, size_t bufferSize) {
    if (bufferSize < 9) return false;
    
    TimeData data = getCurrentTime();
    if (!data.isValid) return false;
    
    snprintf(buffer, bufferSize, "%02d:%02d:%02d", data.hour, data.minute, data.second);
    return true;
}
