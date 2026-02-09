# Testing Guide for WaveRider

## Unit Testing

This project is designed for embedded hardware and requires physical components for full testing. However, you can perform various levels of testing:

## 1. Build Verification

Test that the code compiles correctly:

```bash
./build.sh
```

Expected output: `waverider.uf2` file created successfully.

## 2. Static Analysis

### Check for syntax errors:

```bash
# Using gcc (if available)
gcc -fsyntax-only -I/path/to/pico-sdk/src/common/pico_base/include \
    -I/path/to/pico-sdk/src/rp2_common/pico_platform/include \
    src/*.c
```

### Check code style:

```bash
# Using clang-format (if available)
clang-format -i src/*.c src/*.h
```

## 3. Hardware Testing (Requires Physical Setup)

### Test Setup Checklist:

- [ ] Raspberry Pi Pico connected via USB
- [ ] HC-SR04 sonar sensor wired correctly
- [ ] Meshtastic radio connected via UART
- [ ] All ground connections verified
- [ ] Power supply adequate for all components

### Testing Procedure:

1. **Flash the Firmware**
   ```bash
   # Hold BOOTSEL and connect Pico
   cp build/waverider.uf2 /path/to/PICO_DRIVE/
   ```

2. **Monitor Serial Output**
   ```bash
   # Linux/Mac
   screen /dev/ttyACM0 115200
   
   # Windows (PowerShell)
   # Use PuTTY or TeraTerm
   ```

3. **Verify Initialization**
   - Check for "Sonar sensor initialized successfully" message
   - Check for "Meshtastic UART initialized successfully" message
   - Verify configuration values are displayed correctly

4. **Test Sonar Sensor**
   - Place object in front of sensor
   - Verify distance readings are accurate
   - Expected: Distance updates every transmit interval
   - Compare with physical measurements

5. **Test Meshtastic Communication**
   - Monitor Meshtastic network with another device
   - Verify JSON messages are received
   - Check message format is valid JSON
   - Verify channel and encryption work correctly

6. **Test Status Polling**
   - Observe radio status queries
   - Verify battery level is reported (if supported by radio)
   - Check RSSI values are reasonable

### Expected Serial Output:

```
========================================
WaveRider - Meshtastic Sonar Transmitter
========================================

Initializing sonar sensor on pins 2 (trigger) and 3 (echo)...
Sonar sensor initialized successfully
Initializing Meshtastic UART on pins 4 (TX) and 5 (RX) at 115200 baud...
Meshtastic UART initialized successfully

Configuration:
  Transmit interval: 300000 ms (5 minutes)
  Status poll interval: 60000 ms
  Channel index: 0
  Node ID: 0xFFFFFFFF (broadcast)

Getting initial radio status...
Radio status: Battery=75%, RSSI=-80 dBm, Channel Util=20%
Starting periodic tasks...
System running. Waiting for scheduled events...

Sonar reading: 45.67 cm at 5123 ms
JSON payload: {"sensor":"sonar","distance_cm":45.67,"timestamp":5123,"radio_status":{"battery_level":75,"rssi":-80,"channel_util":20}}
Sent to Meshtastic: AT+SEND=0,FFFFFFFF,{...}
Data transmitted successfully
```

## 4. Component Testing

### Sonar Sensor Only:

Create a minimal test by modifying `main.c` to only test sonar:

```c
int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    sonar_init(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN);
    
    while (true) {
        float distance = sonar_read_distance_cm();
        printf("Distance: %.2f cm\n", distance);
        sleep_ms(1000);
    }
}
```

### UART Communication Only:

Test UART loopback by connecting TX to RX temporarily.

## 5. Configuration Testing

Test different configurations by modifying `config.h`:

1. **Short interval test**: Set `TRANSMIT_INTERVAL_MS` to 10000 (10 seconds)
2. **Different pins**: Change GPIO pin assignments
3. **Different channels**: Test with various channel indices

## 6. Edge Cases

Test the following scenarios:

- [ ] Sonar sensor timeout (no object in range)
- [ ] Radio disconnected or unpowered
- [ ] Rapid distance changes
- [ ] Extended operation (24+ hours)
- [ ] Power cycling
- [ ] USB disconnect/reconnect

## 7. Performance Testing

- Monitor memory usage
- Check for memory leaks (long-term operation)
- Verify timing accuracy of periodic events
- Measure power consumption

## 8. Integration Testing with Real Meshtastic Network

1. Configure Meshtastic radio with matching channel
2. Set up receiving node
3. Verify end-to-end data flow
4. Test range and reliability
5. Monitor for dropped messages

## Troubleshooting Tests

If issues occur:

1. **No sonar readings**: Test sensor with multimeter, verify 5V supply
2. **No UART data**: Use logic analyzer to verify TX/RX signals
3. **Incorrect distances**: Calibrate timing, check for interference
4. **Radio not responding**: Test radio separately with CLI tools

## Known Limitations

- Current implementation uses simplified AT-command protocol (not official Meshtastic protocol)
- For production, should implement proper Protocol Buffers
- Status polling may not work with all Meshtastic firmware versions
- Requires manual radio configuration for channels and encryption

## Next Steps for Production

1. Implement official Meshtastic Protocol Buffers (protobufs)
2. Add proper error handling and recovery
3. Implement watchdog timer
4. Add data persistence (logging)
5. Add OTA (Over-The-Air) update capability
6. Implement proper power management
7. Add unit tests for JSON formatting
8. Create automated hardware-in-the-loop tests
