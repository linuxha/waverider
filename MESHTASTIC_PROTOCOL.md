# Meshtastic UART Protocol Notes

## Overview

This implementation uses a simplified AT-command-style protocol for communication with the Meshtastic radio. In a production environment, you should use the official Meshtastic Protocol Buffers over serial.

## Protocol Buffer Implementation (Recommended)

For production use, integrate the official Meshtastic protobufs:
1. Add Protocol Buffers library (nanopb for embedded systems)
2. Include Meshtastic protobuf definitions
3. Implement proper serialization/deserialization

## Current Implementation

The current implementation uses a simplified text-based protocol for demonstration:

### Send Message
```
AT+SEND=<channel>,<destination_id>,<message>\r\n
```

### Request Status
```
AT+STATUS?\r\n
```

### Expected Response Format
```
BATT=<percentage>,RSSI=<dbm>,UTIL=<percentage>\r\n
```

## Integration with Real Meshtastic Device

To integrate with a real Meshtastic device:

1. **Serial Configuration**: Ensure your Meshtastic device has serial output enabled:
   - Use the Meshtastic CLI or app to enable serial API
   - Set serial baud rate to 115200 (or match your config)

2. **Protocol Buffers**: Replace the simplified protocol with proper protobuf messages:
   - Text messages: Use `Data` protobuf with `portnum = TEXT_MESSAGE_APP`
   - Status queries: Use appropriate admin messages
   
3. **Message Structure**:
   ```
   [START_BYTE][LENGTH][PROTOBUF_DATA][END_BYTE]
   ```

## References

- Official Meshtastic API: https://meshtastic.org/docs/development/device/client-api
- Protobuf definitions: https://github.com/meshtastic/protobufs
- Serial API documentation: https://meshtastic.org/docs/development/device/serial-api
