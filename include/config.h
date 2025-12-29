/**
 * @file config.h
 * @brief Hardware and system configuration for WMATA Metro Monitor
 *
 * This file contains pin definitions for the HUB75 LED matrix panel,
 * display panel configuration, and NTP time sync settings.
 *
 * Note: WiFi credentials (WIFI_SSID, WIFI_PASSWORD), WMATA_API_KEY, and
 * STATION_CODE are loaded from the .env file via load_env.py build script.
 */

#ifndef CONFIG_H
#define CONFIG_H

// =============================================================================
// HUB75 LED Matrix Pin Configuration
// =============================================================================
// These pins connect the ESP32 to the HUB75 LED matrix panel.
// Adjust these if your wiring differs from the default configuration.

// RGB pins for top half of the matrix
#define R1_PIN  13
#define G1_PIN  22
#define B1_PIN  21

// RGB pins for bottom half of the matrix
#define R2_PIN  14
#define G2_PIN  23
#define B2_PIN  27

// Row address pins
#define A_PIN   26
#define B_PIN   15
#define C_PIN   25
#define D_PIN   18
#define E_PIN   -1  // Not needed for 32-pixel height panels

// Control pins
#define LAT_PIN 19  // Latch
#define OE_PIN  32  // Output Enable
#define CLK_PIN 33  // Clock

// =============================================================================
// LED Matrix Panel Configuration
// =============================================================================

/** Panel width in pixels */
#define PANEL_RES_X 64

/** Panel height in pixels */
#define PANEL_RES_Y 32

/** Number of panels chained together */
#define PANEL_CHAIN 1

// =============================================================================
// NTP Time Sync Configuration
// =============================================================================

/** Primary NTP server */
#define NTP_SERVER_PRIMARY   "pool.ntp.org"

/** Secondary NTP server (fallback) */
#define NTP_SERVER_SECONDARY "time.nist.gov"

/** Timezone offset in seconds (EST = UTC-5 = -18000) */
#define TIMEZONE_OFFSET_SEC  -18000

/** Daylight Saving Time offset in seconds (1 hour = 3600) */
#define DST_OFFSET_SEC       3600

#endif // CONFIG_H
