#ifndef CONFIG_H
#define CONFIG_H

// Sonar Sensor Configuration (HC-SR04)
#define SONAR_TRIGGER_PIN 2
#define SONAR_ECHO_PIN 3
#define SONAR_TIMEOUT_US 30000  // 30ms timeout for sonar

// Meshtastic UART Configuration
#define MESHTASTIC_UART_ID uart1
#define MESHTASTIC_UART_TX_PIN 4
#define MESHTASTIC_UART_RX_PIN 5
#define MESHTASTIC_BAUD_RATE 115200

// Transmission Configuration
#define TRANSMIT_INTERVAL_MS 300000  // 5 minutes (configurable)
#define STATUS_POLL_INTERVAL_MS 60000  // Poll radio status every minute

// Meshtastic Channel Configuration
#define MESHTASTIC_CHANNEL_INDEX 0  // Private channel index (0-7)
#define MESHTASTIC_NODE_ID 0xFFFFFFFF  // Broadcast or specific node ID

// Buffer Sizes
#define JSON_BUFFER_SIZE 512
#define UART_BUFFER_SIZE 256

#endif // CONFIG_H
