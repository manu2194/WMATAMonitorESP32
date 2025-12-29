# 🚇 WMATA Metro Monitor

A real-time DC Metro train arrival display built with an ESP32 and a 64x32 LED matrix panel. Shows the next arriving trains for any WMATA station directly from the official WMATA API.

![Display Example](https://img.shields.io/badge/Display-64x32_LED_Matrix-blue)
![Platform](https://img.shields.io/badge/Platform-ESP32-red)
![License](https://img.shields.io/badge/License-MIT-green)

## ⚡ Quick Start

### 1. Get Your WMATA API Key
1. Go to [WMATA Developer Portal](https://developer.wmata.com/)
2. Create a free account
3. Subscribe to the **Default Tier** (free, 10 calls/second)
4. Copy your **Primary Key** from the profile page

### 2. Find Your Station Code
1. Visit the [WMATA Station List API](https://developer.wmata.com/docs/services/5476364f031f590f38092507/operations/5476364f031f5909e4fe3310/console)
2. Look up your station and note the `Code` field (e.g., `B35` for NoMA-Gallaudet U)

### 3. Configure the Project
Create a `.env` file in the project root:
```bash
WMATA_API_KEY=your_api_key_here
STATION_CODE=B35
WIFI_SSID=your-wifi-name
WIFI_PASSWORD=your-wifi-password
```

### 4. Flash the ESP32
```bash
# Install PlatformIO CLI (if not already installed)
pip install platformio

# Build and upload
pio run --target upload

# Monitor serial output (optional)
pio device monitor
```

That's it! Your display should now show real-time train arrivals.

---

## 📖 How It Works

This project turns an ESP32 microcontroller into a real-time metro display:

1. **Connects to WiFi** - The ESP32 connects to your home network
2. **Fetches Train Data** - Every 30 seconds, it calls the [WMATA Real-Time Rail Predictions API](https://developer.wmata.com/docs/services/547636a6f9182302184cda78/operations/547636a6f918230da855363f)
3. **Parses the Response** - Extracts the next arriving train from each direction (Group 1 & 2)
4. **Displays on LED Matrix** - Shows destination names, arrival times, and line colors on the display
5. **Shows Last Update Time** - The bottom of the display shows how long ago the data was refreshed

The display uses the official WMATA metro line colors (Red, Blue, Orange, Green, Yellow, Silver) to color-code train information.

---

## 🔧 Hardware Requirements

| Component | Description | Link |
|-----------|-------------|------|
| **ESP32** | ESP-WROOM-32 Development Board | [Amazon](https://www.amazon.com/gp/product/B08D5ZD528/) |
| **LED Matrix** | 64x32 RGB LED Matrix Panel (HUB75/HUB75E) | [Amazon](https://www.amazon.com/gp/product/B0BRBGHFKQ/) |
| **Power Supply** | 5V 4A (or higher) DC adapter | Required for LED matrix |
| **Jumper Wires** | Male-to-female dupont wires | For connecting ESP32 to matrix |

### Wiring Diagram

Connect the ESP32 to the HUB75 LED matrix panel as follows:

| LED Matrix Pin | ESP32 GPIO |
|----------------|------------|
| R1 | GPIO 13 |
| G1 | GPIO 22 |
| B1 | GPIO 21 |
| R2 | GPIO 14 |
| G2 | GPIO 23 |
| B2 | GPIO 27 |
| A | GPIO 26 |
| B | GPIO 15 |
| C | GPIO 25 |
| D | GPIO 18 |
| E | Not connected (-1) |
| LAT | GPIO 19 |
| OE | GPIO 32 |
| CLK | GPIO 33 |
| GND | GND |

> **Note**: Power the LED matrix directly from a 5V supply, not from the ESP32's 5V pin. The matrix draws significant current that the ESP32 cannot provide.

---

## 📦 Installation

### Install PlatformIO

**Option 1: VS Code Extension (Recommended)**
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Open VS Code and go to Extensions (Ctrl+Shift+X / Cmd+Shift+X)
3. Search for "PlatformIO IDE" and install it
4. Restart VS Code when prompted

**Option 2: CLI Only**
```bash
# Using pip (Python required)
pip install platformio

# Verify installation
pio --version
```

For more options, see the [PlatformIO Installation Guide](https://platformio.org/install).

### Clone and Setup

```bash
# Clone the repository
git clone https://github.com/yourusername/WMATAMonitorESP32.git
cd WMATAMonitorESP32

# Create your .env file
cp .env.example .env  # Or create manually

# Edit .env with your API key, station code, and WiFi credentials
```

### Build and Upload

```bash
# Build the project
pio run

# Build and upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200
```

---

## 🔑 Getting a WMATA API Key

1. **Create an Account**
   - Visit [developer.wmata.com](https://developer.wmata.com/)
   - Click "Sign Up" in the top right
   - Complete the registration form

2. **Subscribe to an API Product**
   - After signing in, go to [Products](https://developer.wmata.com/Products)
   - Click on **Default Tier** (free tier)
   - Click "Subscribe"
   - Accept the terms

3. **Get Your Key**
   - Go to your [Profile](https://developer.wmata.com/profile)
   - Under "Subscriptions", find your Default Tier subscription
   - Click "Show" next to Primary Key
   - Copy this key—this is your `WMATA_API_KEY`

> **Rate Limits**: The free tier allows 10 calls per second. This project refreshes every 30 seconds, well within limits.

---

## 🗺️ Station Codes

Each WMATA station has a unique code. Some stations have multiple codes (one per line served). Use the code for your preferred line.

<details>
<summary><strong>Click to expand full station list</strong></summary>

| Station | Code |
|---------|------|
| Addison Road-Seat Pleasant | G03 |
| Anacostia | F06 |
| Archives-Navy Memorial-Penn Quarter | F02 |
| Arlington Cemetery | C06 |
| Ashburn | N12 |
| Ballston-MU | K04 |
| Benning Road | G01 |
| Bethesda | A09 |
| Braddock Road | C12 |
| Branch Ave | F11 |
| Brookland-CUA | B05 |
| Capitol Heights | G02 |
| Capitol South | D05 |
| Cheverly | D11 |
| Clarendon | K02 |
| Cleveland Park | A05 |
| College Park-U of Md | E09 |
| Columbia Heights | E04 |
| Congress Heights | F07 |
| Court House | K01 |
| Crystal City | C09 |
| Deanwood | D10 |
| Downtown Largo | G05 |
| Dunn Loring-Merrifield | K07 |
| Dupont Circle | A03 |
| East Falls Church | K05 |
| Eastern Market | D06 |
| Eisenhower Avenue | C14 |
| Farragut North | A02 |
| Farragut West | C03 |
| Federal Center SW | D04 |
| Federal Triangle | D01 |
| Foggy Bottom-GWU | C04 |
| Forest Glen | B09 |
| Fort Totten | B06, E06 |
| Franconia-Springfield | J03 |
| Friendship Heights | A08 |
| Gallery Pl-Chinatown | B01, F01 |
| Georgia Ave-Petworth | E05 |
| Glenmont | B11 |
| Greenbelt | E10 |
| Greensboro | N03 |
| Grosvenor-Strathmore | A11 |
| Herndon | N08 |
| Huntington | C15 |
| Hyattsville Crossing | E08 |
| Innovation Center | N09 |
| Judiciary Square | B02 |
| King St-Old Town | C13 |
| L'Enfant Plaza | D03, F03 |
| Landover | D12 |
| Loudoun Gateway | N11 |
| McLean | N01 |
| McPherson Square | C02 |
| Medical Center | A10 |
| Metro Center | A01, C01 |
| Minnesota Ave | D09 |
| Morgan Boulevard | G04 |
| Mt Vernon Sq 7th St-Convention Center | E01 |
| Navy Yard-Ballpark | F05 |
| Naylor Road | F09 |
| New Carrollton | D13 |
| NoMa-Gallaudet U | B35 |
| North Bethesda | A12 |
| Pentagon | C07 |
| Pentagon City | C08 |
| Potomac Ave | D07 |
| Potomac Yard | C11 |
| Reston Town Center | N07 |
| Rhode Island Ave-Brentwood | B04 |
| Rockville | A14 |
| Ronald Reagan Washington National Airport | C10 |
| Rosslyn | C05 |
| Shady Grove | A15 |
| Shaw-Howard U | E02 |
| Silver Spring | B08 |
| Smithsonian | D02 |
| Southern Avenue | F08 |
| Spring Hill | N04 |
| Stadium-Armory | D08 |
| Suitland | F10 |
| Takoma | B07 |
| Tenleytown-AU | A07 |
| Twinbrook | A13 |
| Tysons | N02 |
| U Street/African-Amer Civil War Memorial/Cardozo | E03 |
| Union Station | B03 |
| Van Dorn Street | J02 |
| Van Ness-UDC | A06 |
| Vienna/Fairfax-GMU | K08 |
| Virginia Square-GMU | K03 |
| Washington Dulles International Airport | N10 |
| Waterfront | F04 |
| West Falls Church | K06 |
| West Hyattsville | E07 |
| Wheaton | B10 |
| Wiehle-Reston East | N06 |
| Woodley Park-Zoo/Adams Morgan | A04 |

</details>

---

## 📁 Project Structure

```
WMATAMonitorESP32/
├── src/
│   ├── main.cpp           # Main application logic
│   ├── display.cpp        # LED matrix display functions
│   ├── wifi_manager.cpp   # WiFi connection handling
│   ├── wmata_client.cpp   # WMATA API client
│   ├── relative_time.cpp  # Time formatting utilities
│   └── time_utils.cpp     # Time utilities
├── include/
│   ├── config.h           # WiFi and hardware configuration
│   ├── display.h          # Display class header
│   ├── wifi_manager.h     # WiFi manager header
│   ├── wmata_client.h     # WMATA client header
│   └── ...
├── test/                  # Unit tests
├── .env                   # Your API key and station code (gitignored)
├── load_env.py            # Script to load .env into build
└── platformio.ini         # PlatformIO configuration
```

---

## ⚙️ Configuration Reference

### Environment Variables (`.env`)

| Variable | Required | Default | Description |
|----------|----------|---------|-------------|
| `WMATA_API_KEY` | Yes | - | Your WMATA Developer API key |
| `STATION_CODE` | No | `B35` | Station code to monitor |
| `WIFI_SSID` | Yes | - | Your WiFi network name |
| `WIFI_PASSWORD` | Yes | - | Your WiFi password |

### Hardware Configuration (`include/config.h`)

| Setting | Default | Description |
|---------|---------|-------------|
| `PANEL_RES_X` | 64 | LED matrix width in pixels |
| `PANEL_RES_Y` | 32 | LED matrix height in pixels |
| `PANEL_CHAIN` | 1 | Number of chained panels |

### Application Settings (`src/main.cpp`)

| Setting | Default | Description |
|---------|---------|-------------|
| `REFRESH_INTERVAL_MS` | 30000 | API refresh interval (30 seconds) |

---

## 🧪 Running Tests

This project includes unit tests that can run on your computer (no hardware needed):

```bash
# Run all tests on native platform
pio test -e native

# Run tests on ESP32 (requires connected device)
pio test -e esp32dev_test
```

---

## 🐛 Troubleshooting

### "WiFi Failed!" on display
- Double-check your SSID and password in `.env`
- Ensure your WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Move the device closer to your router

### "API Error" on display
- Verify your `WMATA_API_KEY` is correct in `.env`
- Check that you've subscribed to the Default Tier on the WMATA developer portal
- The WMATA API may be temporarily down—try again later

### "No trains" on display
- This is normal if no trains are arriving at your station
- Late at night when Metro is closed, this is expected

### Display shows garbage or nothing
- Check all wiring connections
- Ensure the LED matrix has adequate power (5V 4A recommended)
- Verify the pin definitions in `config.h` match your wiring

### Build fails with missing environment variable
- Make sure `.env` file exists in the project root
- Check that `WMATA_API_KEY=your_key` is on its own line

---

## 📚 Libraries Used

| Library | Purpose |
|---------|---------|
| [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA) | LED matrix display driver |
| [ArduinoJson](https://arduinojson.org/) | JSON parsing for API responses |
| [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) | Graphics primitives |

---

## 📄 License

This project is open-source and licensed under the MIT License. Feel free to modify and use it for your own projects.

---

## 🙏 Acknowledgments

- [WMATA](https://wmata.com/) for providing the public API
- [mrcodetastic](https://github.com/mrcodetastic) for the excellent ESP32 LED matrix library