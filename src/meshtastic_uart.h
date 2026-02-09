#ifndef MESHTASTIC_UART_H
#define MESHTASTIC_UART_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Meshtastic radio status structure
 */
typedef struct {
    uint32_t battery_level;      // Battery level percentage (0-100)
    int32_t rssi;                 // Signal strength
    uint32_t channel_utilization; // Channel utilization percentage
    bool valid;                   // Whether this status is valid
} meshtastic_status_t;

/**
 * Initialize Meshtastic UART communication
 * @param uart UART instance to use
 * @param tx_pin TX pin number
 * @param rx_pin RX pin number
 * @param baud_rate Baud rate for communication
 * @return true if initialization successful
 */
bool meshtastic_init(uart_inst_t *uart, uint tx_pin, uint rx_pin, uint baud_rate);

/**
 * Send text message over Meshtastic
 * @param channel_index Channel index (0-7)
 * @param destination_id Destination node ID (0xFFFFFFFF for broadcast)
 * @param message Message to send
 * @return true if message sent successfully
 */
bool meshtastic_send_text(uint8_t channel_index, uint32_t destination_id, const char *message);

/**
 * Request radio status from Meshtastic device
 * @param status Pointer to status structure to fill
 * @return true if status retrieved successfully
 */
bool meshtastic_get_status(meshtastic_status_t *status);

/**
 * Process any incoming data from the radio
 * Should be called periodically
 */
void meshtastic_process_incoming(void);

#endif // MESHTASTIC_UART_H
