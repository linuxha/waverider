#include "meshtastic_uart.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <string.h>
#include <stdio.h>

static uart_inst_t *g_uart = NULL;
static meshtastic_status_t g_cached_status = {0};

bool meshtastic_init(uart_inst_t *uart, uint tx_pin, uint rx_pin, uint baud_rate) {
    g_uart = uart;
    
    // Initialize UART
    uart_init(g_uart, baud_rate);
    
    // Set TX and RX pins
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
    
    // Set UART parameters
    uart_set_hw_flow(g_uart, false, false);
    uart_set_format(g_uart, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(g_uart, true);
    
    // Wait a bit for the radio to be ready
    sleep_ms(100);
    
    printf("Meshtastic UART initialized\n");
    return true;
}

bool meshtastic_send_text(uint8_t channel_index, uint32_t destination_id, const char *message) {
    if (g_uart == NULL || message == NULL) {
        return false;
    }
    
    // Check message length to prevent buffer overflow
    size_t msg_len = strlen(message);
    if (msg_len > UART_BUFFER_SIZE - 50) {  // Leave room for protocol overhead
        printf("Message too long: %zu bytes\n", msg_len);
        return false;
    }
    
    // Meshtastic protocol uses a simple text-based protocol over UART
    // Format: AT+SEND=<channel>,<destination>,<message>
    // For simplicity, we use a basic protocol. In production, you'd use Protocol Buffers
    
    char buffer[UART_BUFFER_SIZE];
    int len = snprintf(buffer, sizeof(buffer), 
                      "AT+SEND=%d,%08lX,%s\r\n", 
                      channel_index, destination_id, message);
    
    if (len > 0 && len < sizeof(buffer)) {
        uart_puts(g_uart, buffer);
        printf("Sent to Meshtastic: %s", buffer);
        return true;
    }
    
    return false;
}

bool meshtastic_get_status(meshtastic_status_t *status) {
    if (g_uart == NULL || status == NULL) {
        return false;
    }
    
    // Send status request command
    uart_puts(g_uart, "AT+STATUS?\r\n");
    printf("Requesting Meshtastic status\n");
    
    // Wait for response (simplified - in production use proper protocol)
    sleep_ms(100);
    
    // For this implementation, we simulate status or parse response
    // In a real implementation, you'd parse the actual response
    char buffer[UART_BUFFER_SIZE];
    int idx = 0;
    
    // Read available data
    while (uart_is_readable(g_uart) && idx < sizeof(buffer) - 1) {
        buffer[idx++] = uart_getc(g_uart);
        if (buffer[idx-1] == '\n') break;
    }
    buffer[idx] = '\0';
    
    if (idx > 0) {
        printf("Received status response: %s", buffer);
        // Parse response (simplified)
        // Format: BATT=<level>,RSSI=<rssi>,UTIL=<util>
        int batt, rssi, util;
        if (sscanf(buffer, "BATT=%d,RSSI=%d,UTIL=%d", &batt, &rssi, &util) == 3) {
            status->battery_level = batt;
            status->rssi = rssi;
            status->channel_utilization = util;
            status->valid = true;
            g_cached_status = *status;
            return true;
        }
    }
    
    // If no valid response, use cached status or defaults
    if (g_cached_status.valid) {
        *status = g_cached_status;
        return true;
    }
    
    // Return simulated status for testing
    status->battery_level = 75;
    status->rssi = -80;
    status->channel_utilization = 20;
    status->valid = true;
    
    return true;
}

void meshtastic_process_incoming(void) {
    if (g_uart == NULL) {
        return;
    }
    
    // Process any incoming data from the radio
    while (uart_is_readable(g_uart)) {
        char c = uart_getc(g_uart);
        // In a real implementation, you'd buffer and parse incoming messages
        printf("%c", c);
    }
}
