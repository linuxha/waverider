#ifndef SONAR_SENSOR_H
#define SONAR_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Initialize the sonar sensor (HC-SR04)
 * @param trigger_pin GPIO pin for trigger
 * @param echo_pin GPIO pin for echo
 * @return true if initialization successful
 */
bool sonar_init(uint trigger_pin, uint echo_pin);

/**
 * Read distance from sonar sensor
 * @return Distance in centimeters, or -1.0 if measurement failed
 */
float sonar_read_distance_cm(void);

/**
 * Get the last successful measurement timestamp
 * @return Timestamp in milliseconds since boot
 */
uint32_t sonar_get_last_measurement_time(void);

#endif // SONAR_SENSOR_H
