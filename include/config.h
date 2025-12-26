#ifndef CONFIG_H
#define CONFIG_H

/*
 * WiFi Configuration
 */
#define WIFI_SSID "SingleDigits - Resident"
#define WIFI_PASSWORD "PadSavingsSchema"

/*
 * Timezone Configuration
 * EST: UTC-5, DST offset: 1 hour
 */
#define TIMEZONE_OFFSET_SEC (-5 * 3600)
#define DST_OFFSET_SEC 3600
#define NTP_SERVER_PRIMARY "pool.ntp.org"
#define NTP_SERVER_SECONDARY "time.nist.gov"

/*
 * LED Matrix Panel Configuration
 */
#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1

/*
 * LED Matrix Pin Definitions
 */
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
#define E_PIN -1
#define LAT_PIN 19
#define OE_PIN 32
#define CLK_PIN 33

#endif // CONFIG_H
