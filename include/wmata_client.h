#ifndef WMATA_CLIENT_H
#define WMATA_CLIENT_H

#include <Arduino.h>

/**
 * Maximum number of trains to store/display
 */
#define MAX_TRAINS 2

/**
 * Maximum length for destination name (first 3 letters only for LED display)
 */
#define DEST_MAX_LEN 4

/**
 * Maximum length for minutes string ("ARR", "BRD", or number)
 */
#define MIN_MAX_LEN 4

/**
 * Maximum length for line code (e.g., "RD", "BL")
 */
#define LINE_MAX_LEN 3

/**
 * Structure to hold a single train prediction
 */
struct TrainPrediction {
    char destination[DEST_MAX_LEN];  // Truncated destination name
    char minutes[MIN_MAX_LEN];       // Minutes until arrival ("ARR", "BRD", or number)
    char line[LINE_MAX_LEN];         // Line code (RD, BL, OR, etc.)
};

/**
 * WMATA API client for fetching real-time train predictions
 * 
 * Example usage:
 * ```cpp
 * WmataClient client("B35", WMATA_API_KEY);
 * if (client.fetchPredictions()) {
 *     TrainPrediction train = client.getTrain(0);
 *     Serial.printf("%s - %s min\n", train.destination, train.minutes);
 * }
 * ```
 */
class WmataClient {
public:
    /**
     * Constructor
     * 
     * :param const char* stationCode: WMATA station code (e.g., "B35" for NoMA)
     * :param const char* apiKey: WMATA API key
     */
    WmataClient(const char* stationCode, const char* apiKey);
    
    /**
     * Fetch train predictions from WMATA API
     * 
     * :return bool: True if fetch was successful, false otherwise
     */
    bool fetchPredictions();
    
    /**
     * Get the number of trains currently stored (max 2)
     * 
     * :return int: Number of trains (0-2)
     */
    int getTrainCount() const;
    
    /**
     * Get a train prediction by index
     * 
     * :param int index: Train index (0 or 1)
     * :return TrainPrediction: The train prediction data
     */
    TrainPrediction getTrain(int index) const;
    
    /**
     * Get the timestamp of the last successful fetch
     * 
     * :return unsigned long: millis() value when last fetched, 0 if never
     */
    unsigned long getLastFetchTime() const;
    
    /**
     * Get the station code this client is configured for
     * 
     * :return const char*: Station code string
     */
    const char* getStationCode() const;

private:
    char _stationCode[8];
    char _apiKey[64];
    TrainPrediction _trains[MAX_TRAINS];
    int _trainCount;
    unsigned long _lastFetchTime;
    
    /**
     * Parse a single train JSON object into TrainPrediction
     * 
     * :param JsonObject& trainObj: The JSON object for a train
     * :param TrainPrediction& prediction: Output prediction struct
     */
    void _parseTrainObject(const char* destination, const char* minutes, const char* line, TrainPrediction& prediction);
};

#endif // WMATA_CLIENT_H
