# WMATA Monitor ESP32

This project provides the source code for an ESP32 module that connects to a 64x32 LED display. The ESP32 module is designed to connect to a Wi-Fi network and retrieve WMATA metro timing predictions from a webserver. The display then shows the real-time metro schedule.

While this project is functional, it contains some hardcoded assumptions based on the specific setup of the metro timing server. These include the expected format of the server's response, which is provided by a Flask webserver running on a Raspberry Pi. Therefore, the project is not fully generic and may require adjustments if used in a different context. However, it serves as a useful reference for how to control a 64x32 LED matrix display with an ESP32.

## Features

- Acts as an HTTP Client and reach out to a web server to retrieve real-time WMATA train predictions. 
- Displays predictions on a 64x32 LED matrix.
- Displays the last time the data was updated

## Prerequsites
- ESP-WROOM-32 ([Amazon Link](https://www.amazon.com/gp/product/B08D5ZD528/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1))
- 64x32 2048 RGB Full Color LED Matrix Panel ([Amazon Link](https://www.amazon.com/gp/product/B0BRBGHFKQ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1))
  - Must be a HUB75 / HUB75E connection based RGB LED panel
  - The LED write operations are significantly simplified by using this library, which is fully compatible with the hardware I purchased from the provided link. You can find the library here: [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA).
- A server that returns metro timings in the following format
```json
{
    // must be a list of exactly two strings
    "line": [
        "Sta1 5,10",
        "Sta2 4,9"
    ],

    // timestamp must be in ISO format
    "timestamp": "2024-10-12T21:20:03.143040"
}
```
## Libraries

## Project Structure

- `src/`: Contains the main source code for the ESP32 module.
- `platformio.ini`: Configuration file for the PlatformIO


## Installation and Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/esp32-metro-timing-display.git
   cd esp32-metro-timing-display
   ```

2. **Install PlatformIO**:
   Follow the instructions to install PlatformIO from [here](https://platformio.org/install).

3. **Connect your ESP32**:
   Plug your ESP32 module into your computer via USB.

4. **Configure Wi-Fi credentials**:
   Open the `src/main.cpp` file and update the Wi-Fi credentials and server URL to match your setup:
   ```cpp
   const char* ssid = "your-SSID";
   const char* password = "your-PASSWORD";
   const char* serverURL = "http://your-flask-server/api/timings";
   ```

5. **Flash the firmware**:
   Use PlatformIO to compile and upload the firmware to your ESP32:
   ```bash
   pio run --target upload
   ```

6. **Run the Flask Server**:
   Ensure that your Flask server is running and returning the metro timing predictions in the expected format.

## Usage

Once the ESP32 module is flashed and running:

- It will connect to the specified Wi-Fi network.
- It will make a request to the Flask webserver running on your Raspberry Pi.
- The LED display will show the real-time metro schedule based on the server's response.

## Limitations

- **Hardcoded Server Response Format**: The current code assumes that the webserver's response follows a specific format designed for this project. To use the code in a different context, you may need to modify how the response is parsed.
- **Non-Generic**: This code is designed specifically for a particular metro timing system. It may not work as-is for other use cases without modification.
- **No Detailed Instructions**: This project assumes a working knowledge of ESP32, PlatformIO, and LED matrix displays.

## Inspiration

This project is a great starting point if you are looking to learn how to interface an ESP32 with a 64x32 LED matrix display. While it may not be directly applicable to all projects, it demonstrates the basic principles of connecting to a web server and displaying data on an LED matrix.

## License

This project is open-source and licensed under the MIT License. Feel free to modify and use it as inspiration for your own projects.

---

**Disclaimer**: This project is provided as-is, with no detailed instructions on adapting it to different use cases. It was developed for a specific purpose, and modifications will likely be required for other applications.