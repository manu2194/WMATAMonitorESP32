#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

/**
 * WifiManager class to handle WiFi connection
 */
class WifiManager {
public:
    WifiManager();
    
    /**
     * Connect to WiFi network using credentials from config
     * 
     * :param unsigned long timeoutMs: Connection timeout in milliseconds
     * :return bool: True if connected successfully
     */
    bool connect(unsigned long timeoutMs = 30000);
    
    /**
     * Check if WiFi is currently connected
     * 
     * :return bool: True if connected
     */
    bool isConnected();
    
    /**
     * Get the local IP address as a string
     * 
     * :return String: The IP address
     */
    String getIPAddress();
};

#endif // WIFI_MANAGER_H
