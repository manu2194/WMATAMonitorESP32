#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Fonts/Picopixel.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <ArduinoJson.h>
#include <time.h>
#include <HTTPClient.h>

/*
*********************************************
* WiFi Related variables
*********************************************
*/
// Replace with your network credentials
const char *ssid = "SingleDigits - Resident";
const char *password = "PadSavingsSchema";
const char *hostname = "esp32-wmata-monitor";
const char *apiEndpointUrl = "http://guardianape.local:5000";

/*
*********************************************
* Web Related variables
*********************************************
*/

/******************************************** */

/*
*********************************************
* LED Matrix Related variables
*********************************************
*/
MatrixPanel_I2S_DMA *dma_display = nullptr;
uint16_t red = dma_display->color565(255, 0, 0);
uint16_t blue = dma_display->color565(0, 255, 0);
uint16_t green = dma_display->color565(0, 0, 255);
uint16_t white = dma_display->color565(255, 255, 255);
uint16_t black = dma_display->color565(0, 0, 0);

const int MAX_LINES = 10; // Adjust based on expected number of lines
const int MAX_LINE_LENGTH = 100; // Adjust based on maximum line length

// LED Matrix code adapted
// from https://stackoverflow.com/questions/77131504/arduino-nano-esp-32-with-64x64-led-display-using-esp32-hub75-matrixpanel-i2s-dma
#define PANEL_RES_X 64 // Number of pixels wide of each INDIVIDUAL panel module.
#define PANEL_RES_Y 32 // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1  // Total number of panels chained one to another

// Pin connections for the LED Matrix
#define R1_PIN 13
#define G1_PIN 22
#define B1_PIN 21
#define R2_PIN 14
#define G2_PIN 23
#define B2_PIN 27
#define A_PIN 26
#define B_PIN 15
#define C_PIN 25
#define D_PIN 18
#define E_PIN -1 // required for 1/32 scan panels, like 64x64px. Any available pin would do, i.e. IO32
#define LAT_PIN 19
#define OE_PIN 32
#define CLK_PIN 33
/******************************************** */


// Last fetched timestamp in Unix format
time_t lastFetchedTimestamp = 0;

/**
 * Get a relative time string for a given past timestamp.
 * @param pastTimestamp The past timestamp to calculate the relative time from.
 * @return A string representing the relative time, e.g., "5 seconds ago", "3 hours ago".
 */
std::string getRelativeTimeString(time_t pastTimestamp) {
    time_t currentTime = time(nullptr);
    time_t diff = currentTime - pastTimestamp;

    if (diff < 60) {
        return std::to_string(diff) + " s ago";
    } else if (diff < 3600) {
        return std::to_string(diff / 60) + " m ago";
    } else if (diff < 86400) {
        return std::to_string(diff / 3600) + " h ago";
    } else if (diff < 2592000) { // Less than 30 days
        return std::to_string(diff / 86400) + " d ago";
    } else if (diff < 31104000) { // Less than 12 months
        return std::to_string(diff / 2592000) + " M ago";
    } else {
        return std::to_string(diff / 31104000) + " y ago";
    }
}


void setPinModes()
{
  // Setting PIN modes of ESP32 to OUTPUT
  pinMode(R1_PIN, OUTPUT);
  pinMode(G1_PIN, OUTPUT);
  pinMode(B1_PIN, OUTPUT);
  pinMode(R2_PIN, OUTPUT);
  pinMode(G2_PIN, OUTPUT);
  pinMode(B2_PIN, OUTPUT);
  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(C_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  // pinMode(E_PIN, OUTPUT);  // commenting this out because it's not needed for a single panel LED Matrix
  pinMode(LAT_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
}

void initWifi()
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to");
  Serial.println(ssid);

  /**
   * * The following is to set the DNS name of this device to `<hostname>.local` so that any device
   * * on the local network can resolve this ESP32 with `<hostname>.local`
   */
  // Initialize mDNS
  if (!MDNS.begin(hostname))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }

  // Print local IP address and start web server
  Serial.print("\nESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
  { 
    // This loop will continuously run until the serial connection is established 
  }

  setPinModes();

  // Module configuration
  // Settings pins to HUB75
  HUB75_I2S_CFG::i2s_pins _pins = {R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
  HUB75_I2S_CFG mxconfig(
      PANEL_RES_X, // module width
      PANEL_RES_Y, // module height
      PANEL_CHAIN, // Chain length
      _pins);

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->clearScreen();

  dma_display->setTextColor(white);
  dma_display->setCursor(0, 0);
  dma_display->print("Connecting to Wi-Fi...");
  initWifi();


  // Synchronize time using NTP server
  configTime(0, 0, "pool.ntp.org");
  dma_display->clearScreen();
  dma_display->setTextColor(green);
  dma_display->setCursor(0, 0);
  dma_display->print("Time Synchronized");
}

void loop() {
    static unsigned long lastApiFetchMillis = 0;
    unsigned long currentTimeMillis = millis();

    // Fetch from API every 5 seconds
    if (currentTimeMillis - lastApiFetchMillis >= 5000 || lastApiFetchMillis == 0) {
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient httpClient;
            httpClient.begin(apiEndpointUrl);
            int httpStatusCode = httpClient.GET();

            if (httpStatusCode == 200) {
                dma_display->clearScreen();
                dma_display->setFont(nullptr);
                String apiResponsePayload = httpClient.getString();
                StaticJsonDocument<200> doc;
                DeserializationError error = deserializeJson(doc, apiResponsePayload);
                if (!error) {
                    JsonArray lineArray = doc["line"].as<JsonArray>();
                    dma_display->setTextColor(red);
                    dma_display->setCursor(1, 0);
                    dma_display->print(lineArray[0].as<const char*>());
                    dma_display->setCursor(1, 9);
                    dma_display->print(lineArray[1].as<const char*>());

                    const char* timestampStr = doc["timestamp"];
                    struct tm timeInfo;
                    if (strptime(timestampStr, "%Y-%m-%dT%H:%M:%S", &timeInfo)) {
                        lastFetchedTimestamp = mktime(&timeInfo);
                    }
                } else {
                    dma_display->clearScreen();
                    dma_display->setTextColor(red);
                    dma_display->setCursor(0, 0);
                    dma_display->print("PARSE ERROR");
                }
            } else {
              dma_display->clearScreen();
              dma_display->setTextColor(red);
              dma_display->setCursor(0, 0);
              dma_display->print("API ERROR");
            }
            httpClient.end();
        } else {
            dma_display->clearScreen();
            dma_display->setTextColor(red);
            dma_display->setCursor(0, 0);
            dma_display->print("NO WIFI");
        }
        lastApiFetchMillis = currentTimeMillis;
    }

    // Print the relative time every second
    if (lastFetchedTimestamp != 0) {
        dma_display->fillRect(0, 24, dma_display->width(), 8, black); // this is like a mini-clear screen
        dma_display->setTextColor(white);
        dma_display->setCursor(1, 24);
        dma_display->write(getRelativeTimeString(lastFetchedTimestamp).c_str());
    }
    delay(1000);
}