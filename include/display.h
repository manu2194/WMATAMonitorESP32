#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

/**
 * Display class to manage the HUB75 LED matrix panel
 */
class Display {
public:
    Display();
    
    /**
     * Initialize the LED matrix display
     * 
     * :return bool: True if initialization successful
     */
    bool init();
    
    /**
     * Clear the entire display
     */
    void clear();
    
    /**
     * Show a simple text message on the display
     * 
     * :param const char* message: The message to display
     * :param uint16_t color: The color to use (use color565 format)
     */
    void showMessage(const char* message, uint16_t color);
    
    /**
     * Display time in HH:MM:SS format with AM/PM indicator
     * 
     * :param int hour: Hour in 12-hour format (1-12)
     * :param int minute: Minutes (0-59)
     * :param int second: Seconds (0-59)
     * :param bool isPM: True for PM, false for AM
     */
    void showTime(int hour, int minute, int second, bool isPM);
    
    /**
     * Display metro train arrivals
     * 
     * Shows up to 2 trains with their destinations and arrival times,
     * plus a "last updated" timestamp.
     * 
     * Format:
     *   | {Dest1}  - {Min1} |
     *   | {Dest2}  - {Min2} |
     *   | {X} {u} ago       |
     * 
     * :param const char* train1Dest: First train destination (or nullptr if no trains)
     * :param const char* train1Min: First train minutes (or nullptr)
     * :param const char* train2Dest: Second train destination (or nullptr)
     * :param const char* train2Min: Second train minutes (or nullptr)
     * :param const char* lastUpdated: "X s ago" or "X m ago" string
     * :param uint16_t lineColor: Color for the train line indicator
     */
    void showMetroArrivals(const char* train1Dest, const char* train1Min,
                           const char* train2Dest, const char* train2Min,
                           const char* lastUpdated, uint16_t lineColor);
    
    /**
     * Get the raw display pointer for advanced operations
     * 
     * :return MatrixPanel_I2S_DMA*: Pointer to the display object
     */
    MatrixPanel_I2S_DMA* getRaw();
    
    // Color helper
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

private:
    MatrixPanel_I2S_DMA* _display;
    uint16_t _colorWhite;
    uint16_t _colorBlack;
    uint16_t _colorCyan;
    
    void _setPinModes();
};

#endif // DISPLAY_H
