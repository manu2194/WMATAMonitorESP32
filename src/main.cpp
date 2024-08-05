#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// Base code adapted from https://stackoverflow.com/questions/77131504/arduino-nano-esp-32-with-64x64-led-display-using-esp32-hub75-matrixpanel-i2s-dma

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

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

 
MatrixPanel_I2S_DMA *dma_display = nullptr;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    // This loop will continuously run until the serial connection is established
  }

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
  pinMode(E_PIN, OUTPUT);
  pinMode(LAT_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);


  HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};

  // Module configuration
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN,    // Chain length
    _pins
  );

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);


  dma_display->begin();
  dma_display->clearScreen();
  uint16_t myWHITE = dma_display->color565(255, 0, 0);
  dma_display->fillScreen(myWHITE);
}

void loop() {
  // put your main code here, to run repeatedly:

}