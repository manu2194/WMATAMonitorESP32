#include "wmata_client.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Base URL for WMATA StationPrediction API
static const char* WMATA_API_BASE_URL = "http://api.wmata.com/StationPrediction.svc/json/GetPrediction/";

WmataClient::WmataClient(const char* stationCode, const char* apiKey) {
    strncpy(_stationCode, stationCode, sizeof(_stationCode) - 1);
    _stationCode[sizeof(_stationCode) - 1] = '\0';
    
    strncpy(_apiKey, apiKey, sizeof(_apiKey) - 1);
    _apiKey[sizeof(_apiKey) - 1] = '\0';
    
    _trainCount = 0;
    _lastFetchTime = 0;
    
    // Initialize trains array
    for (int i = 0; i < MAX_TRAINS; i++) {
        _trains[i].destination[0] = '\0';
        _trains[i].minutes[0] = '\0';
        _trains[i].line[0] = '\0';
    }
}

bool WmataClient::fetchPredictions() {
    HTTPClient http;
    
    // Build the full URL
    String url = String(WMATA_API_BASE_URL) + _stationCode + 
                 "?contentType=application/json&api_key=" + _apiKey;
    
    Serial.println("[WMATA] Fetching predictions...");
    Serial.print("[WMATA] URL: ");
    Serial.println(url);
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("[WMATA] HTTP error: %d\n", httpCode);
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    Serial.println("[WMATA] Response received, parsing...");
    
    // Parse JSON response
    // The response can be quite large, allocate enough memory
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.printf("[WMATA] JSON parse error: %s\n", error.c_str());
        return false;
    }
    
    // Reset train count
    _trainCount = 0;
    
    // Get the Trains array
    JsonArray trains = doc["Trains"].as<JsonArray>();
    
    if (trains.isNull()) {
        Serial.println("[WMATA] No Trains array in response");
        return false;
    }
    
    // Track which groups we've seen (to get one train per direction)
    // Group "1" and "2" represent opposite directions at a station
    bool seenGroup1 = false;
    bool seenGroup2 = false;
    
    // Process trains - get the first train from each group
    // WMATA returns trains sorted by arrival time, so first occurrence
    // of each group is the next train in that direction
    for (JsonObject train : trains) {
        if (_trainCount >= MAX_TRAINS) break;
        
        const char* destination = train["Destination"] | "";
        const char* minutes = train["Min"] | "";
        const char* line = train["Line"] | "";
        const char* group = train["Group"] | "";
        
        // Skip trains with empty or invalid data
        if (strlen(destination) == 0 || strlen(minutes) == 0) {
            continue;
        }
        
        // Check if we've already got a train for this group
        bool isGroup1 = (strcmp(group, "1") == 0);
        bool isGroup2 = (strcmp(group, "2") == 0);
        
        if (isGroup1 && seenGroup1) continue;  // Already have Group 1 train
        if (isGroup2 && seenGroup2) continue;  // Already have Group 2 train
        
        // Store this train
        _parseTrainObject(destination, minutes, line, _trains[_trainCount]);
        _trainCount++;
        
        // Mark group as seen
        if (isGroup1) seenGroup1 = true;
        if (isGroup2) seenGroup2 = true;
        
        Serial.printf("[WMATA] Selected train for Group %s: %s - %s min\n", 
                      group, destination, minutes);
    }
    
    _lastFetchTime = millis();
    
    Serial.printf("[WMATA] Parsed %d trains (one per direction)\n", _trainCount);
    for (int i = 0; i < _trainCount; i++) {
        Serial.printf("[WMATA]   Train %d: %s - %s min (Line %s)\n", 
                      i + 1, _trains[i].destination, _trains[i].minutes, _trains[i].line);
    }
    
    return true;
}

int WmataClient::getTrainCount() const {
    return _trainCount;
}

TrainPrediction WmataClient::getTrain(int index) const {
    if (index >= 0 && index < _trainCount) {
        return _trains[index];
    }
    // Return empty prediction for invalid index
    TrainPrediction empty;
    empty.destination[0] = '\0';
    empty.minutes[0] = '\0';
    empty.line[0] = '\0';
    return empty;
}

unsigned long WmataClient::getLastFetchTime() const {
    return _lastFetchTime;
}

const char* WmataClient::getStationCode() const {
    return _stationCode;
}

void WmataClient::_parseTrainObject(const char* destination, const char* minutes, const char* line, TrainPrediction& prediction) {
    // Copy destination (truncated to fit LED display)
    strncpy(prediction.destination, destination, DEST_MAX_LEN - 1);
    prediction.destination[DEST_MAX_LEN - 1] = '\0';
    
    // Copy minutes
    strncpy(prediction.minutes, minutes, MIN_MAX_LEN - 1);
    prediction.minutes[MIN_MAX_LEN - 1] = '\0';
    
    // Copy line code
    strncpy(prediction.line, line, LINE_MAX_LEN - 1);
    prediction.line[LINE_MAX_LEN - 1] = '\0';
}
