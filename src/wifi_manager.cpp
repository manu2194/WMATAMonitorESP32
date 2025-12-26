#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>

WifiManager::WifiManager() {}

bool WifiManager::connect(unsigned long timeoutMs) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting to WiFi");
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime > timeoutMs) {
            Serial.println("\nWiFi connection timeout!");
            return false;
        }
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
    
    return true;
}

bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WifiManager::getIPAddress() {
    return WiFi.localIP().toString();
}
