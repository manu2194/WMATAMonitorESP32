#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <Fonts/Picopixel.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <ArduinoJson.h>
/*
*********************************************
* WiFi Related variables
*********************************************
*/
// Replace with your network credentials
const char *ssid = "SingleDigits - Resident";
const char *password = "PadSavingsSchema";
const char *hostname = "esp32-wmata-monitor";

/*
*********************************************
* Web Related variables
*********************************************
*/
AsyncWebServer webServer(80);

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

void getNotFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void getHome(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", "Hello");
}


void postTrainTimings(AsyncWebServerRequest *request)
{
  String data;
  int params = request->params();
  for (int i=0;i<params;i++){
    AsyncWebParameter *p = request->getParam(i);
    if(p->isPost()){
      data = p->value().c_str();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, data);
      if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
      }
      const char* line_0_name = doc["line"][0]["name"];
      const char* line_0_destinations_0 = doc["line"][0]["destinations"][0];
      const char* line_0_destinations_1 = doc["line"][0]["destinations"][1];
      const char* timestamp = doc["timestamp"];

      Serial.println(line_0_name);
      Serial.println(line_0_destinations_0);
      Serial.println(line_0_destinations_1);
      Serial.println(timestamp);
      
      dma_display->clearScreen();
      dma_display->setFont(nullptr);
      dma_display->setTextColor(red);

      dma_display->setCursor(0, 0);
      dma_display->print(line_0_destinations_0);

      dma_display->setCursor(0, 9);
      dma_display->print(line_0_destinations_1);


      dma_display->setTextColor(white);
      dma_display->setCursor(0, 24);
      dma_display->setFont(&Picopixel);
      dma_display->print(timestamp);
    }
  }
  request->send(200, "text/plain", "Success");
}

void initWebServer()
{
  webServer.on("/", HTTP_GET, getHome);
  webServer.on("/", HTTP_POST, postTrainTimings);
  webServer.onNotFound(getNotFound);

  webServer.begin();
  Serial.print("\nStarted Web Server");
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
  dma_display->print("Starting Server...");
  initWebServer();
  dma_display->clearScreen();
  dma_display->setTextColor(green);
  dma_display->setCursor(0, 0);
  dma_display->print("Ready!");
}

void loop()
{
}