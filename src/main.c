/**
 * WaveRider - Pico Meshtastic Sonar Data Transmitter
 * 
 * This application reads data from a sonar sensor (HC-SR04) and transmits
 * the data periodically via a Meshtastic radio connected over UART.
 * It also polls the radio for status information (battery level, etc.)
 * and includes that in the transmission.
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/timer.h"

#include "config.h"
#include "sonar_sensor.h"
#include "meshtastic_uart.h"
#include "json_utils.h"

// Alarm IDs for periodic tasks
static alarm_id_t transmit_alarm_id = 0;
static alarm_id_t status_poll_alarm_id = 0;

// Cached radio status
static meshtastic_status_t radio_status = {0};

/**
 * Callback for periodic data transmission
 */
int64_t transmit_callback(alarm_id_t id, void *user_data) {
    // Read sonar sensor
    float distance = sonar_read_distance_cm();
    uint32_t timestamp = sonar_get_last_measurement_time();
    
    if (distance >= 0) {
        printf("Sonar reading: %.2f cm at %lu ms\n", distance, timestamp);
        
        // Create JSON message with both sonar and radio status
        char json_buffer[JSON_BUFFER_SIZE];
        int json_len = json_create_combined_data(json_buffer, sizeof(json_buffer),
                                                 distance, timestamp, &radio_status);
        
        if (json_len > 0) {
            printf("JSON payload: %s\n", json_buffer);
            
            // Send via Meshtastic
            bool sent = meshtastic_send_text(MESHTASTIC_CHANNEL_INDEX, 
                                            MESHTASTIC_NODE_ID, 
                                            json_buffer);
            
            if (sent) {
                printf("Data transmitted successfully\n");
            } else {
                printf("Failed to transmit data\n");
            }
        } else {
            printf("Failed to create JSON payload\n");
        }
    } else {
        printf("Sonar sensor read failed\n");
    }
    
    // Reschedule the alarm
    return TRANSMIT_INTERVAL_MS * 1000;  // Return value in microseconds
}

/**
 * Callback for periodic status polling
 */
int64_t status_poll_callback(alarm_id_t id, void *user_data) {
    // Poll radio for status
    if (meshtastic_get_status(&radio_status)) {
        printf("Radio status: Battery=%lu%%, RSSI=%ld dBm, Channel Util=%lu%%\n",
               radio_status.battery_level, radio_status.rssi, 
               radio_status.channel_utilization);
    } else {
        printf("Failed to get radio status\n");
    }
    
    // Reschedule the alarm
    return STATUS_POLL_INTERVAL_MS * 1000;  // Return value in microseconds
}

/**
 * Main application entry point
 */
int main() {
    // Initialize standard I/O
    stdio_init_all();
    
    // Wait for USB serial connection (optional, comment out if not needed)
    sleep_ms(2000);
    
    printf("\n========================================\n");
    printf("WaveRider - Meshtastic Sonar Transmitter\n");
    printf("========================================\n\n");
    
    // Initialize sonar sensor
    printf("Initializing sonar sensor on pins %d (trigger) and %d (echo)...\n",
           SONAR_TRIGGER_PIN, SONAR_ECHO_PIN);
    if (!sonar_init(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN)) {
        printf("ERROR: Failed to initialize sonar sensor\n");
        return 1;
    }
    printf("Sonar sensor initialized successfully\n");
    
    // Initialize Meshtastic UART
    printf("Initializing Meshtastic UART on pins %d (TX) and %d (RX) at %d baud...\n",
           MESHTASTIC_UART_TX_PIN, MESHTASTIC_UART_RX_PIN, MESHTASTIC_BAUD_RATE);
    if (!meshtastic_init(MESHTASTIC_UART_ID, MESHTASTIC_UART_TX_PIN, 
                        MESHTASTIC_UART_RX_PIN, MESHTASTIC_BAUD_RATE)) {
        printf("ERROR: Failed to initialize Meshtastic UART\n");
        return 1;
    }
    printf("Meshtastic UART initialized successfully\n");
    
    // Print configuration
    printf("\nConfiguration:\n");
    printf("  Transmit interval: %d ms (%.1f minutes)\n", 
           TRANSMIT_INTERVAL_MS, TRANSMIT_INTERVAL_MS / 60000.0f);
    printf("  Status poll interval: %d ms\n", STATUS_POLL_INTERVAL_MS);
    printf("  Channel index: %d\n", MESHTASTIC_CHANNEL_INDEX);
    printf("  Node ID: 0x%08lX (broadcast)\n\n", MESHTASTIC_NODE_ID);
    
    // Get initial radio status
    printf("Getting initial radio status...\n");
    meshtastic_get_status(&radio_status);
    
    // Schedule periodic tasks
    printf("Starting periodic tasks...\n");
    transmit_alarm_id = add_alarm_in_ms(5000, transmit_callback, NULL, false);
    status_poll_alarm_id = add_alarm_in_ms(2000, status_poll_callback, NULL, false);
    
    printf("System running. Waiting for scheduled events...\n\n");
    
    // Main loop
    while (true) {
        // Process any incoming data from the radio
        meshtastic_process_incoming();
        
        // Small delay to prevent busy waiting
        sleep_ms(100);
        
        // Optional: Add watchdog or other monitoring here
    }
    
    return 0;
}
