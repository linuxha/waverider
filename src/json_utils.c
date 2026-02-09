#include "json_utils.h"
#include <stdio.h>
#include <string.h>

int json_create_sonar_data(char *buffer, size_t buffer_size, 
                           float distance, uint32_t timestamp) {
    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }
    
    int len = snprintf(buffer, buffer_size,
                      "{\"sensor\":\"sonar\",\"distance_cm\":%.2f,\"timestamp\":%lu}",
                      distance, timestamp);
    
    if (len < 0 || len >= buffer_size) {
        return -1;
    }
    
    return len;
}

int json_create_status_data(char *buffer, size_t buffer_size, 
                            const meshtastic_status_t *status) {
    if (buffer == NULL || buffer_size == 0 || status == NULL) {
        return -1;
    }
    
    int len = snprintf(buffer, buffer_size,
                      "{\"radio_status\":{\"battery_level\":%lu,\"rssi\":%ld,\"channel_util\":%lu}}",
                      status->battery_level, status->rssi, status->channel_utilization);
    
    if (len < 0 || len >= buffer_size) {
        return -1;
    }
    
    return len;
}

int json_create_combined_data(char *buffer, size_t buffer_size,
                              float distance, uint32_t timestamp,
                              const meshtastic_status_t *status) {
    if (buffer == NULL || buffer_size == 0 || status == NULL) {
        return -1;
    }
    
    int len = snprintf(buffer, buffer_size,
                      "{\"sensor\":\"sonar\",\"distance_cm\":%.2f,\"timestamp\":%lu,"
                      "\"radio_status\":{\"battery_level\":%lu,\"rssi\":%ld,\"channel_util\":%lu}}",
                      distance, timestamp,
                      status->battery_level, status->rssi, status->channel_utilization);
    
    if (len < 0 || len >= buffer_size) {
        return -1;
    }
    
    return len;
}
