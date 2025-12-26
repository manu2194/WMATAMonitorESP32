#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "wifi_manager.h"
#include "time_utils.h"

// Global instances
Display display;
WifiManager wifi;
TimeManager timeManager;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        // Wait for serial connection
    }
    
    // Initialize display
    display.init();
    display.showMessage("Starting...", display.color565(255, 255, 255));
    
    // Connect to WiFi
    display.clear();
    display.showMessage("Connecting...", display.color565(255, 255, 255));
    
    if (!wifi.connect()) {
        display.clear();
        display.showMessage("WiFi Failed!", display.color565(255, 0, 0));
        while (1) { delay(1000); }
    }
    
    // Sync time
    display.clear();
    display.showMessage("Syncing time...", display.color565(255, 255, 255));
    timeManager.syncNTP();
    
    // Wait for time to sync
    delay(2000);
}

void loop() {
    TimeData currentTime = timeManager.getCurrentTime();
    
    if (currentTime.isValid) {
        display.showTime(
            currentTime.hour,
            currentTime.minute,
            currentTime.second,
            currentTime.isPM
        );
    } else {
        display.clear();
        display.showMessage("Time sync...", display.color565(255, 255, 0));
    }
    
    delay(1000);
}
