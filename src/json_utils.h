#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <stdint.h>
#include "meshtastic_uart.h"

/**
 * Create JSON string with sonar sensor data
 * @param buffer Buffer to store JSON string
 * @param buffer_size Size of the buffer
 * @param distance Distance measurement in cm
 * @param timestamp Timestamp of measurement
 * @return Number of characters written, or -1 on error
 */
int json_create_sonar_data(char *buffer, size_t buffer_size, 
                           float distance, uint32_t timestamp);

/**
 * Create JSON string with radio status data
 * @param buffer Buffer to store JSON string
 * @param buffer_size Size of the buffer
 * @param status Radio status structure
 * @return Number of characters written, or -1 on error
 */
int json_create_status_data(char *buffer, size_t buffer_size, 
                            const meshtastic_status_t *status);

/**
 * Create combined JSON string with both sonar and status data
 * @param buffer Buffer to store JSON string
 * @param buffer_size Size of the buffer
 * @param distance Distance measurement in cm
 * @param timestamp Timestamp of measurement
 * @param status Radio status structure
 * @return Number of characters written, or -1 on error
 */
int json_create_combined_data(char *buffer, size_t buffer_size,
                              float distance, uint32_t timestamp,
                              const meshtastic_status_t *status);

#endif // JSON_UTILS_H
