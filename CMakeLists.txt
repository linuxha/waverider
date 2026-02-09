cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(waverider C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add main executable
add_executable(waverider
    src/main.c
    src/sonar_sensor.c
    src/meshtastic_uart.c
    src/json_utils.c
)

# Pull in common dependencies
target_link_libraries(waverider 
    pico_stdlib
    hardware_uart
    hardware_gpio
    hardware_timer
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(waverider)

# Enable USB output, disable UART output
pico_enable_stdio_usb(waverider 1)
pico_enable_stdio_uart(waverider 0)
