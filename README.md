# WaveRider

Raspberry Pi Pico application for reading sonar sensor data and transmitting it periodically via a Meshtastic radio over UART.

## Features

- **Sonar Sensor Reading**: Reads distance measurements from HC-SR04 ultrasonic sensor
- **Periodic Transmission**: Configurable interval for data transmission (default: 5 minutes)
- **JSON Data Format**: Transmits sensor data in JSON format
- **Radio Status Monitoring**: Polls Meshtastic radio for battery level and other status information
- **Configurable Private Channel**: Supports configurable Meshtastic channel index
- **UART Communication**: Communicates with Meshtastic radio via UART

## Hardware Requirements

- Raspberry Pi Pico (or Pico W)
- Meshtastic-compatible radio (e.g., T-Beam, T-Echo, etc.)
- HC-SR04 Ultrasonic Sonar Sensor
- Jumper wires

## Wiring Diagram

### Sonar Sensor (HC-SR04)
- VCC → 5V (VBUS pin 40)
- GND → GND (any GND pin)
- TRIG → GPIO 2 (configurable in config.h)
- ECHO → GPIO 3 (configurable in config.h)

### Meshtastic Radio (UART)
- Radio TX → Pico GPIO 5 (RX - configurable in config.h)
- Radio RX → Pico GPIO 4 (TX - configurable in config.h)
- Radio GND → Pico GND
- Radio VCC → Pico 3V3 (or external power if needed)

## Configuration

Edit `src/config.h` to customize:

```c
// Transmit interval (milliseconds)
#define TRANSMIT_INTERVAL_MS 300000  // 5 minutes

// Meshtastic channel index (0-7)
#define MESHTASTIC_CHANNEL_INDEX 0

// GPIO pins
#define SONAR_TRIGGER_PIN 2
#define SONAR_ECHO_PIN 3
#define MESHTASTIC_UART_TX_PIN 4
#define MESHTASTIC_UART_RX_PIN 5
```

## Building

### Prerequisites

1. Install the Raspberry Pi Pico SDK
2. Set the `PICO_SDK_PATH` environment variable

```bash
export PICO_SDK_PATH=/path/to/pico-sdk
```

### Build Steps

```bash
mkdir build
cd build
cmake ..
make
```

This will generate `waverider.uf2` file.

## Flashing

1. Hold the BOOTSEL button on your Pico while connecting it to your computer
2. Copy the `waverider.uf2` file to the mounted Pico drive
3. The Pico will automatically reboot and run the application

## Usage

1. Connect the sonar sensor and Meshtastic radio as per the wiring diagram
2. Power on the Pico
3. The application will:
   - Initialize the sonar sensor and radio
   - Start reading the sonar sensor at configured intervals
   - Transmit JSON data to the Meshtastic network
   - Poll the radio for status information

## Data Format

The application transmits data in JSON format:

```json
{
  "sensor": "sonar",
  "distance_cm": 45.67,
  "timestamp": 123456,
  "radio_status": {
    "battery_level": 75,
    "rssi": -80,
    "channel_util": 20
  }
}
```

## Monitoring

Connect to the Pico via USB serial (115200 baud) to see debug output:

```bash
# Linux/Mac
screen /dev/ttyACM0 115200

# Or use minicom
minicom -D /dev/ttyACM0 -b 115200
```

## Troubleshooting

### Sonar sensor not reading
- Check wiring connections
- Ensure VCC is connected to 5V (VBUS)
- Verify ECHO voltage is safe for GPIO (use voltage divider if needed)

### No data transmitted
- Verify Meshtastic radio is powered and configured
- Check UART connections (TX/RX not swapped)
- Ensure radio is on the same channel as configured

### Build errors
- Ensure Pico SDK is properly installed
- Verify `PICO_SDK_PATH` environment variable is set
- Try `PICO_SDK_FETCH_FROM_GIT=1` to auto-download SDK

## License

See LICENSE file for details.
