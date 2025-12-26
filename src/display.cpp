#include "display.h"
#include "config.h"

Display::Display() : _display(nullptr) {}

bool Display::init() {
    _setPinModes();
    
    HUB75_I2S_CFG::i2s_pins _pins = {
        R1_PIN, G1_PIN, B1_PIN,
        R2_PIN, G2_PIN, B2_PIN,
        A_PIN, B_PIN, C_PIN, D_PIN, E_PIN,
        LAT_PIN, OE_PIN, CLK_PIN
    };
    
    HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN, _pins);
    
    _display = new MatrixPanel_I2S_DMA(mxconfig);
    _display->begin();
    _display->clearScreen();
    
    // Initialize colors
    _colorWhite = _display->color565(255, 255, 255);
    _colorBlack = _display->color565(0, 0, 0);
    _colorCyan = _display->color565(0, 255, 255);
    
    return true;
}

void Display::_setPinModes() {
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
    pinMode(LAT_PIN, OUTPUT);
    pinMode(OE_PIN, OUTPUT);
    pinMode(CLK_PIN, OUTPUT);
}

void Display::clear() {
    if (_display) {
        _display->clearScreen();
    }
}

void Display::showMessage(const char* message, uint16_t color) {
    if (!_display) return;
    
    _display->setTextColor(color);
    _display->setCursor(0, 0);
    _display->print(message);
}

void Display::showTime(int hour, int minute, int second, bool isPM) {
    if (!_display) return;
    
    char timeStr[12];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hour, minute, second);
    
    _display->clearScreen();
    
    // Draw time in cyan
    _display->setTextColor(_colorCyan);
    _display->setCursor(5, 8);
    _display->print(timeStr);
    
    // Draw AM/PM in white
    _display->setTextColor(_colorWhite);
    _display->setCursor(22, 20);
    _display->print(isPM ? "PM" : "AM");
}

MatrixPanel_I2S_DMA* Display::getRaw() {
    return _display;
}

uint16_t Display::color565(uint8_t r, uint8_t g, uint8_t b) {
    if (_display) {
        return _display->color565(r, g, b);
    }
    return 0;
}

void Display::showMetroArrivals(const char* train1Dest, const char* train1Min,
                                 const char* train2Dest, const char* train2Min,
                                 const char* lastUpdated, uint16_t lineColor) {
    if (!_display) return;
    
    _display->clearScreen();
    
    // Row height spacing for 32-pixel tall display
    // Line 1: y = 2
    // Line 2: y = 12
    // Line 3: y = 22
    
    // Display first train (if available)
    if (train1Dest != nullptr && train1Min != nullptr) {
        char line1[24];
        snprintf(line1, sizeof(line1), "%s - %s", train1Dest, train1Min);
        
        _display->setTextColor(lineColor);
        _display->setCursor(1, 2);
        _display->print(line1);
    } else {
        _display->setTextColor(_colorWhite);
        _display->setCursor(1, 2);
        _display->print("No trains");
    }
    
    // Display second train (if available)
    if (train2Dest != nullptr && train2Min != nullptr) {
        char line2[24];
        snprintf(line2, sizeof(line2), "%s - %s", train2Dest, train2Min);
        
        _display->setTextColor(lineColor);
        _display->setCursor(1, 12);
        _display->print(line2);
    }
    
    // Display last updated time at bottom
    if (lastUpdated != nullptr) {
        _display->setTextColor(_colorWhite);
        _display->setCursor(1, 24);
        _display->print(lastUpdated);
    }
}
