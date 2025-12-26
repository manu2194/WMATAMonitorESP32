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
