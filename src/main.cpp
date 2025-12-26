#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "wifi_manager.h"
#include "wmata_client.h"
#include "relative_time.h"

// WMATA_API_KEY is defined via build flags from .env file
// See load_env.py for details
/**
 * Station Configuration
 * Change these values to monitor a different station
 */
#define STATION_CODE "B35"           // NoMA-Gallaudet U
#define STATION_NAME "NoMa-Gallaudet U"

/**
 * Refresh interval for metro data (in milliseconds)
 */
#define REFRESH_INTERVAL_MS 30000  // 30 seconds

/**
 * Line colors for WMATA metro lines
 */
#define LINE_COLOR_RD 0xF800  // Red Line
#define LINE_COLOR_BL 0x001F  // Blue Line
#define LINE_COLOR_OR 0xFC00  // Orange Line
#define LINE_COLOR_GR 0x07E0  // Green Line
#define LINE_COLOR_YL 0xFFE0  // Yellow Line
#define LINE_COLOR_SV 0xC618  // Silver Line

// Global instances
Display display;
WifiManager wifi;
WmataClient wmataClient(STATION_CODE, WMATA_API_KEY);

// Timing variables
unsigned long lastFetchTime = 0;

/**
 * Get the appropriate color for a metro line
 * 
 * :param const char* lineCode: The line code (RD, BL, OR, GR, YL, SV)
 * :return uint16_t: 565-format color value
 */
uint16_t getLineColor(const char* lineCode) {
    if (strcmp(lineCode, "RD") == 0) return LINE_COLOR_RD;
    if (strcmp(lineCode, "BL") == 0) return LINE_COLOR_BL;
    if (strcmp(lineCode, "OR") == 0) return LINE_COLOR_OR;
    if (strcmp(lineCode, "GR") == 0) return LINE_COLOR_GR;
    if (strcmp(lineCode, "YL") == 0) return LINE_COLOR_YL;
    if (strcmp(lineCode, "SV") == 0) return LINE_COLOR_SV;
    return display.color565(255, 255, 255);  // Default white
}

/**
 * Fetch and display metro arrivals
 */
void updateMetroDisplay() {
    Serial.println("[MAIN] Updating metro display...");
    
    if (!wmataClient.fetchPredictions()) {
        Serial.println("[MAIN] Failed to fetch predictions");
        display.clear();
        display.showMessage("API Error", display.color565(255, 0, 0));
        return;
    }
    
    int trainCount = wmataClient.getTrainCount();
    
    if (trainCount == 0) {
        display.clear();
        display.showMessage("No trains", display.color565(255, 255, 0));
        return;
    }
    
    // Get train data
    TrainPrediction train1 = wmataClient.getTrain(0);
    TrainPrediction train2 = wmataClient.getTrain(1);
    
    // Get line color from first train
    uint16_t lineColor = getLineColor(train1.line);
    
    // Calculate relative time since last fetch
    unsigned long elapsedMs = millis() - wmataClient.getLastFetchTime();
    char relativeTime[16];
    formatRelativeTime(elapsedMs, relativeTime, sizeof(relativeTime));
    
    // Display the arrivals
    if (trainCount == 1) {
        display.showMetroArrivals(
            train1.destination, train1.minutes,
            nullptr, nullptr,
            relativeTime, lineColor
        );
    } else {
        display.showMetroArrivals(
            train1.destination, train1.minutes,
            train2.destination, train2.minutes,
            relativeTime, lineColor
        );
    }
}

/**
 * Update the "last updated" display without refetching data
 */
void updateRelativeTimeDisplay() {
    int trainCount = wmataClient.getTrainCount();
    
    if (trainCount == 0) {
        return;  // Nothing to update
    }
    
    TrainPrediction train1 = wmataClient.getTrain(0);
    TrainPrediction train2 = wmataClient.getTrain(1);
    uint16_t lineColor = getLineColor(train1.line);
    
    // Calculate relative time since last fetch
    unsigned long elapsedMs = millis() - wmataClient.getLastFetchTime();
    char relativeTime[16];
    formatRelativeTime(elapsedMs, relativeTime, sizeof(relativeTime));
    
    // Redraw display with updated relative time
    if (trainCount == 1) {
        display.showMetroArrivals(
            train1.destination, train1.minutes,
            nullptr, nullptr,
            relativeTime, lineColor
        );
    } else {
        display.showMetroArrivals(
            train1.destination, train1.minutes,
            train2.destination, train2.minutes,
            relativeTime, lineColor
        );
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        // Wait for serial connection
    }
    
    Serial.println("\n=== WMATA Metro Monitor ===");
    Serial.printf("Station: %s (%s)\n", STATION_NAME, STATION_CODE);
    
    // Initialize display
    display.init();
    display.showMessage("Starting...", display.color565(255, 255, 255));
    
    // Connect to WiFi
    display.clear();
    display.showMessage("Connecting...", display.color565(255, 255, 255));
    
    if (!wifi.connect()) {
        display.clear();
        display.showMessage("WiFi Failed!", display.color565(255, 0, 0));
        Serial.println("[MAIN] WiFi connection failed!");
        while (1) { delay(1000); }
    }
    
    Serial.println("[MAIN] WiFi connected!");
    Serial.print("[MAIN] IP: ");
    Serial.println(wifi.getIPAddress());
    
    // Initial fetch
    display.clear();
    display.showMessage("Fetching...", display.color565(255, 255, 255));
    updateMetroDisplay();
    lastFetchTime = millis();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Refresh data every REFRESH_INTERVAL_MS
    if (currentTime - lastFetchTime >= REFRESH_INTERVAL_MS) {
        updateMetroDisplay();
        lastFetchTime = currentTime;
    } else {
        // Just update the relative time display (every second)
        updateRelativeTimeDisplay();
    }
    
    delay(1000);
}
