#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "wifi_manager.h"
#include "wmata_client.h"
#include "relative_time.h"

// WMATA_API_KEY and STATION_CODE are defined via build flags from .env file
// See load_env.py for details

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

// State tracking
unsigned long lastFetchTime = 0;
bool hasError = false;           // Track if last fetch had an error
const char* errorMessage = "";   // Error message to display

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
 * Sets hasError and errorMessage if fetch fails
 */
void updateMetroDisplay() {
    Serial.println("[MAIN] Updating metro display...");
    
    // Calculate relative time (always shown, even on error)
    unsigned long elapsedMs = millis() - lastFetchTime;
    char relativeTime[16];
    formatRelativeTime(elapsedMs, relativeTime, sizeof(relativeTime));
    
    if (!wmataClient.fetchPredictions()) {
        Serial.println("[MAIN] Failed to fetch predictions");
        hasError = true;
        errorMessage = "API Error";
        
        // Show error but still display the timer
        display.showMetroArrivals(
            "ERR", "!",
            nullptr, nullptr,
            relativeTime, display.color565(255, 0, 0)
        );
        return;
    }
    
    int trainCount = wmataClient.getTrainCount();
    
    if (trainCount == 0) {
        hasError = true;
        errorMessage = "No trains";
        
        display.showMetroArrivals(
            "None", "-",
            nullptr, nullptr,
            relativeTime, display.color565(255, 255, 0)
        );
        return;
    }
    
    // Success! Clear error state
    hasError = false;
    errorMessage = "";
    
    // Get train data
    TrainPrediction train1 = wmataClient.getTrain(0);
    TrainPrediction train2 = wmataClient.getTrain(1);
    
    // Get line color from first train
    uint16_t lineColor = getLineColor(train1.line);
    
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
 * Update the display (timer always updates, data only if no error)
 */
void updateDisplay() {
    // Calculate relative time since last fetch (always shown)
    unsigned long elapsedMs = millis() - lastFetchTime;
    char relativeTime[16];
    formatRelativeTime(elapsedMs, relativeTime, sizeof(relativeTime));
    
    if (hasError) {
        // Still in error state - show error with updated timer
        display.showMetroArrivals(
            "ERR", "!",
            nullptr, nullptr,
            relativeTime, display.color565(255, 0, 0)
        );
        return;
    }
    
    int trainCount = wmataClient.getTrainCount();
    
    if (trainCount == 0) {
        display.showMetroArrivals(
            "None", "-",
            nullptr, nullptr,
            relativeTime, display.color565(255, 255, 0)
        );
        return;
    }
    
    TrainPrediction train1 = wmataClient.getTrain(0);
    TrainPrediction train2 = wmataClient.getTrain(1);
    uint16_t lineColor = getLineColor(train1.line);
    
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
    Serial.printf("Station Code: %s\n", STATION_CODE);
    
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
    lastFetchTime = millis();  // Set time before fetch for accurate timer
    updateMetroDisplay();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Refresh data every REFRESH_INTERVAL_MS
    if (currentTime - lastFetchTime >= REFRESH_INTERVAL_MS) {
        lastFetchTime = currentTime;  // Update time before fetch
        updateMetroDisplay();
    } else {
        // Just update the timer display (every second)
        updateDisplay();
    }
    
    delay(1000);
}
