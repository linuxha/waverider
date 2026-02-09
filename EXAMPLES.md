# Example Configuration Scenarios

## Scenario 1: Quick Polling (Every Minute)

Edit `src/config.h`:
```c
#define TRANSMIT_INTERVAL_MS 60000  // 1 minute
#define STATUS_POLL_INTERVAL_MS 30000  // 30 seconds
```

## Scenario 2: Extended Battery Life (Every 15 Minutes)

Edit `src/config.h`:
```c
#define TRANSMIT_INTERVAL_MS 900000  // 15 minutes
#define STATUS_POLL_INTERVAL_MS 300000  // 5 minutes
```

## Scenario 3: Different GPIO Pins

Edit `src/config.h`:
```c
#define SONAR_TRIGGER_PIN 6
#define SONAR_ECHO_PIN 7
#define MESHTASTIC_UART_TX_PIN 8
#define MESHTASTIC_UART_RX_PIN 9
```

## Scenario 4: Private Channel Configuration

To use a specific private channel (0-7):

Edit `src/config.h`:
```c
#define MESHTASTIC_CHANNEL_INDEX 3  // Use channel 3
```

Make sure your Meshtastic radio is configured with the same channel index and encryption key.

## Scenario 5: Targeted Transmission (Non-Broadcast)

To send to a specific node instead of broadcast:

Edit `src/config.h`:
```c
#define MESHTASTIC_NODE_ID 0x12345678  // Replace with target node ID
```

To find a node ID:
1. Use Meshtastic mobile app
2. Or use the radio's admin commands
3. Node IDs are 32-bit hex values

## Advanced: Multiple Sensor Support

To add more sensors, follow this pattern:

1. Create new sensor file (e.g., `src/temperature_sensor.c`)
2. Add sensor reading in `transmit_callback()` in `main.c`
3. Update `json_utils.c` to include new sensor data
4. Add sensor to CMakeLists.txt

Example JSON with multiple sensors:
```json
{
  "sensors": {
    "sonar": {"distance_cm": 45.67},
    "temperature": {"celsius": 22.5},
    "humidity": {"percent": 65}
  },
  "timestamp": 123456,
  "radio_status": {...}
}
```
