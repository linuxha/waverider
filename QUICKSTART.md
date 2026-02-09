# WaveRider Quick Start Guide

## 📋 What You Need

- Raspberry Pi Pico (or Pico W)
- HC-SR04 Ultrasonic Sonar Sensor
- Meshtastic Radio (T-Beam, T-Echo, etc.)
- Jumper wires
- USB cable (for programming)

## 🔌 Quick Wiring

```
HC-SR04 Sonar:
  VCC  → Pico Pin 40 (5V)
  TRIG → Pico Pin 4 (GP2)
  ECHO → Pico Pin 5 (GP3) ⚠️ Use voltage divider!
  GND  → Pico GND

Meshtastic Radio:
  TX → Pico Pin 7 (GP5)
  RX → Pico Pin 6 (GP4)
  GND → Pico GND
  VCC → 3.3V or External Power
```

⚠️ **IMPORTANT:** ECHO pin outputs 5V but Pico is 3.3V. Use voltage divider or level shifter!

## 🛠️ Build & Flash

```bash
# 1. Set SDK path or auto-download
export PICO_SDK_PATH=/path/to/pico-sdk
# OR let it auto-download:
export PICO_SDK_FETCH_FROM_GIT=1

# 2. Build
./build.sh

# 3. Flash
# Hold BOOTSEL button, connect Pico
# Copy build/waverider.uf2 to Pico drive
```

## ⚙️ Configuration

Edit `src/config.h` before building:

```c
// Change transmit interval (default: 5 minutes)
#define TRANSMIT_INTERVAL_MS 300000

// Change channel (0-7)
#define MESHTASTIC_CHANNEL_INDEX 0

// Change GPIO pins if needed
#define SONAR_TRIGGER_PIN 2
#define SONAR_ECHO_PIN 3
```

## 📊 Data Format

Your Pico sends JSON like this:

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

## 🔍 Monitor Output

```bash
# Linux/Mac
screen /dev/ttyACM0 115200

# Windows
# Use PuTTY or TeraTerm at 115200 baud
```

## ✅ Quick Test

1. Power on → Check USB serial for "initialized successfully"
2. Place object near sonar → Check distance readings
3. Verify data sends every 5 minutes (or configured interval)
4. Check Meshtastic network receives messages

## 🐛 Common Issues

| Problem | Solution |
|---------|----------|
| No sonar readings | Check wiring, ensure 5V to VCC |
| No UART data | Swap TX/RX connections |
| Pico won't boot | Check for short circuits |
| Wrong distances | Verify ECHO voltage divider |

## 📚 Full Documentation

- `README.md` - Complete setup guide
- `WIRING.md` - Detailed wiring with diagrams
- `TESTING.md` - Testing procedures
- `EXAMPLES.md` - Configuration examples
- `SECURITY.md` - Security analysis

## 🚀 Next Steps

1. Test with physical hardware
2. Adjust transmit interval as needed
3. Configure Meshtastic radio channel
4. Deploy and monitor

## 💡 Tips

- Start with short interval (30 seconds) for testing
- Monitor serial output during first tests
- Ensure Meshtastic antenna is connected before power on
- Use common ground for all components
- Consider external power for Meshtastic radio

## 📞 Need Help?

Check the full documentation in the repository or open an issue on GitHub.

---
**Quick Start Version 1.0** | See README.md for details
