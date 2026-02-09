# WaveRider Wiring Guide

## Overview

This guide provides detailed wiring instructions for connecting the Raspberry Pi Pico, HC-SR04 sonar sensor, and Meshtastic radio.

## Component List

1. **Raspberry Pi Pico** (or Pico W)
2. **HC-SR04 Ultrasonic Sensor**
3. **Meshtastic Radio** (e.g., LILYGO T-Beam, T-Echo, Heltec LoRa, etc.)
4. **Breadboard** (optional, for prototyping)
5. **Jumper wires** (male-to-male, male-to-female)
6. **Voltage divider** (optional, for ECHO pin - see safety note)

## Raspberry Pi Pico Pinout Reference

```
                    ┌─────────────┐
                3V3 │1  ●    ● 40 │ VBUS (5V)
      (UART0 TX) GP0│2        39 │ VSYS
      (UART0 RX) GP1│3        38 │ GND
   (SONAR TRIG) GP2 │4        37 │ 3V3_EN
   (SONAR ECHO) GP3 │5        36 │ ADC_VREF
                 GP4│6        35 │ ADC2 (GP28)
                 GP5│7        34 │ ADC1 (GP27)
                 GND│8        33 │ GND
                 GP6│9        32 │ ADC0 (GP26)
                 GP7│10       31 │ RUN
                 GP8│11       30 │ GP22
                 GP9│12       29 │ GND
                GND │13       28 │ GP21
                GP10│14       27 │ GP20
                GP11│15       26 │ GP19
                GP12│16       25 │ GP18
                GP13│17       24 │ GND
                GND │18       23 │ GP17
                GP14│19       22 │ GP16
                GP15│20       21 │ GP15
                    └─────────────┘
```

## Default Pin Assignments (as per config.h)

- **GP2**: Sonar Trigger
- **GP3**: Sonar Echo
- **GP4**: Meshtastic UART TX
- **GP5**: Meshtastic UART RX

## Wiring Instructions

### 1. HC-SR04 Sonar Sensor

The HC-SR04 has 4 pins:

```
┌─────────────────────┐
│  HC-SR04 Ultrasonic │
│                     │
│  VCC  TRIG  ECHO GND│
│   │    │     │    │ │
└───┼────┼─────┼────┼─┘
    │    │     │    │
```

**Connections:**

| HC-SR04 Pin | Connect to Pico           | Notes                      |
|-------------|---------------------------|----------------------------|
| VCC         | Pin 40 (VBUS/5V)          | HC-SR04 requires 5V        |
| TRIG        | Pin 4 (GP2)               | 3.3V signal is sufficient  |
| ECHO        | Pin 5 (GP3)               | ⚠️ See safety note below    |
| GND         | Pin 38 (GND)              | Common ground              |

**⚠️ SAFETY NOTE for ECHO Pin:**

The HC-SR04 ECHO pin outputs 5V, but Pico GPIO pins are 3.3V tolerant. You have two options:

**Option A: Direct Connection (Risky)**
- Connect ECHO directly to GP3
- Most HC-SR04 sensors work, but this is technically out of spec
- May damage the Pico in some cases

**Option B: Voltage Divider (Recommended)**
- Use resistor divider to reduce 5V to 3.3V
- Circuit: HC-SR04 ECHO → R1 (2kΩ) → GP3 → R2 (3.3kΩ) → GND
- Safe for the Pico

**Option C: Level Shifter**
- Use a bi-directional level shifter module
- Most reliable but requires additional component

### 2. Meshtastic Radio UART Connection

Meshtastic radios typically expose UART pins. Common radios:

#### T-Beam / T-Echo / Heltec LoRa:

**Find UART pins on your radio** (consult your radio's documentation):
- Usually labeled as TX, RX, and GND
- Some radios have dedicated serial headers

**Connections:**

| Meshtastic Radio | Connect to Pico           | Notes                      |
|------------------|---------------------------|----------------------------|
| TX (Radio)       | Pin 7 (GP5) - RX (Pico)   | Radio transmits to Pico    |
| RX (Radio)       | Pin 6 (GP4) - TX (Pico)   | Pico transmits to Radio    |
| GND              | Pin 8 or 13 or 38 (GND)   | Common ground              |
| VCC              | Pin 36 (3V3) or external  | See power note below       |

**⚠️ POWER NOTE:**

Some Meshtastic radios draw significant current and should NOT be powered from Pico's 3V3:
- If radio current < 300mA: Can power from Pico 3V3
- If radio has LoRa transmission: Use external power supply
- Always check your radio's specifications

#### Typical T-Beam Pinout Example:
```
T-Beam Board
┌──────────────────┐
│  SX1276/78 LoRa  │
│                  │
│  TX  RX  GND VCC │
│  │   │   │   │   │
└──┼───┼───┼───┼───┘
   │   │   │   │
   │   │   │   └─→ 3.3V or External
   │   │   └─────→ GND
   │   └─────────→ GP5 (Pico RX)
   └─────────────→ GP4 (Pico TX)
```

## Complete Wiring Diagram

```
                        Raspberry Pi Pico
                      ┌───────────────────┐
         ┌────────────│40 VBUS      GP2 4 │────────┐
         │       ┌────│39 VSYS      GP3 5 │───┐    │
         │       │ ┌──│38 GND       GP4 6 │─┐ │    │
         │       │ │  │             GP5 7 │┐│ │    │
         │       │ │  │                   │││ │    │
         │       │ │  │                   │││ │    │
         │       │ │  └───────────────────┘││ │    │
         │       │ │                       ││ │    │
         │       │ └───────────┬───────────┘│ │    │
         │       │             │            │ │    │
    HC-SR04 Sonar              │       Meshtastic Radio
    ┌──────────┐               │       ┌──────────┐
    │ VCC TRIG │               │       │ TX    VCC│
    │ │   │    │               │       │ │     │  │
    │ │   └────┼───────────────┘       │ └─────┼──┘
    │ │        │                       │       │
    │ │   ECHO │                       │ RX    │
    │ │   │    │                       │ │     │
    │ │   └────┼───────────────────────┼─┘  GND│
    │ │        │                       │    │   │
    └─┼────────┴───────────────────────┴────┼───┘
      │                                     │
      └─────────────────┬───────────────────┘
                        │
                    Common GND
```

## Power Considerations

### Option 1: USB Power Only
- Connect Pico via USB (provides 5V on VBUS)
- Sonar powered from VBUS (Pin 40)
- Radio powered from external source or 3V3 if low power

### Option 2: External Power
- Use external 5V power supply
- Connect to VSYS (Pin 39) - accepts 1.8V-5.5V
- Ensure all grounds are common

### Option 3: Battery Power
- Use 3.7V LiPo battery connected to VSYS
- Add voltage regulator for 5V to sonar if needed
- Consider power consumption for battery life

## Verification Checklist

Before powering on:

- [ ] All GND connections are common
- [ ] No short circuits between power rails
- [ ] UART TX/RX not swapped (TX → RX, RX → TX)
- [ ] ECHO pin has voltage divider or level shifter
- [ ] Power supply can handle total current draw
- [ ] Meshtastic radio has appropriate antenna connected
- [ ] No loose connections

## Testing Individual Components

### Test 1: Sonar Only
1. Connect only sonar sensor
2. Flash test firmware
3. Verify distance readings

### Test 2: UART Loopback
1. Temporarily connect GP4 to GP5 (TX to RX)
2. Test UART echo
3. Disconnect before connecting radio

### Test 3: Full System
1. Connect all components
2. Power up in order: Pico → Radio → Sonar
3. Monitor serial output

## Common Issues and Solutions

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No sonar readings | Wiring incorrect | Check TRIG/ECHO pins |
| Pico won't boot | Short circuit | Check power connections |
| UART not working | TX/RX swapped | Swap GP4 and GP5 connections |
| Radio not responding | No power/wrong baud | Check power and config |
| Erratic readings | Poor connections | Use breadboard or solder |

## Advanced: Custom Pin Configuration

To use different pins, edit `src/config.h`:

```c
// Example: Use GP6 and GP7 for sonar
#define SONAR_TRIGGER_PIN 6
#define SONAR_ECHO_PIN 7

// Example: Use GP8 and GP9 for UART
#define MESHTASTIC_UART_TX_PIN 8
#define MESHTASTIC_UART_RX_PIN 9
```

Remember to use `uart0` or `uart1` based on pin selection:
- UART0: GP0-1, GP12-13, GP16-17
- UART1: GP4-5, GP8-9

## Safety Guidelines

1. **Never connect 5V to 3.3V pins directly**
2. **Always use common ground**
3. **Disconnect power before making changes**
4. **Double-check connections before power on**
5. **Use appropriate current-rated wires**
6. **Ensure antenna is connected to radio before transmission**
7. **Monitor for excessive heating**

## Resources

- [Pico Datasheet](https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf)
- [HC-SR04 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
- [Meshtastic Hardware Docs](https://meshtastic.org/docs/hardware)
