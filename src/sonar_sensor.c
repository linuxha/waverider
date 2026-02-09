#include "sonar_sensor.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

static uint g_trigger_pin;
static uint g_echo_pin;
static uint32_t g_last_measurement_time = 0;

bool sonar_init(uint trigger_pin, uint echo_pin) {
    g_trigger_pin = trigger_pin;
    g_echo_pin = echo_pin;
    
    // Initialize trigger pin as output
    gpio_init(g_trigger_pin);
    gpio_set_dir(g_trigger_pin, GPIO_OUT);
    gpio_put(g_trigger_pin, 0);
    
    // Initialize echo pin as input
    gpio_init(g_echo_pin);
    gpio_set_dir(g_echo_pin, GPIO_IN);
    
    return true;
}

float sonar_read_distance_cm(void) {
    // Send 10us trigger pulse
    gpio_put(g_trigger_pin, 1);
    sleep_us(10);
    gpio_put(g_trigger_pin, 0);
    
    // Wait for echo to go high
    uint32_t timeout = time_us_32() + SONAR_TIMEOUT_US;
    while (gpio_get(g_echo_pin) == 0) {
        if (time_us_32() > timeout) {
            return -1.0f;
        }
    }
    
    // Measure pulse width
    uint32_t start_time = time_us_32();
    timeout = start_time + SONAR_TIMEOUT_US;
    
    while (gpio_get(g_echo_pin) == 1) {
        if (time_us_32() > timeout) {
            return -1.0f;
        }
    }
    
    uint32_t end_time = time_us_32();
    uint32_t pulse_width = end_time - start_time;
    
    // Calculate distance in cm
    // Speed of sound is 343 m/s or 0.0343 cm/us
    // Distance = (pulse_width * 0.0343) / 2
    float distance = (float)pulse_width * 0.01715f;
    
    g_last_measurement_time = to_ms_since_boot(get_absolute_time());
    
    return distance;
}

uint32_t sonar_get_last_measurement_time(void) {
    return g_last_measurement_time;
}
