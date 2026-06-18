# ECU Operation

This document describes how the simulated ECU firmware works at a higher level:
which sensors are read, how the fuel maps are indexed, how the engine state is
updated, and how the outputs are driven.

## Main Control Loop

The firmware starts by configuring the PIC16F877A ports, ADC, timers, LCD, and
interrupts. It then reads the last selected fuel map from internal EEPROM and
enters the main loop.

When the ignition switch is active, each loop iteration performs the following
steps:

1. Read buttons and update the user interface state.
2. Read analog sensors through the ADC.
3. Detect whether the starter button is pressed.
4. Calculate the engine logic and injector pulse value.
5. Update auxiliary actuators.
6. Refresh the LCD periodically.

When the ignition switch is off, the firmware disables the pump, fan, starter,
ignition outputs, injectors, and timers.

## Sensor Inputs

The project uses simulated sensors connected to the PIC ADC channels.

| ADC channel | Variable | Simulated signal | Firmware interpretation |
| --- | --- | --- | --- |
| AN0 | `g_adc_tps` | Throttle position sensor | Used as the TPS axis of the fuel map and for deceleration fuel cut |
| AN1 | `g_adc_rpm_sim` | RPM potentiometer/signal | Converted into simulated engine speed and used as the RPM axis |
| AN2 | `g_adc_sonda` | Lambda/oxygen sensor | Converted into an AFR-like value for closed-loop correction |
| AN3 | `g_adc_temp` | Temperature sensor | Converted into degrees Celsius for fan control and warnings |

The ADC returns a 10-bit value from 0 to 1023. The simulated RPM is calculated
as:

```c
g_rpm_real = g_adc_rpm_sim * 8;
```

This maps the ADC range to approximately 0 to 8184 RPM.

The simulated lambda input is converted into an AFR-like range from 10.0 to
20.0:

```c
afr_lido = 10.0 + (g_adc_sonda * 10.0 / 1023.0);
```

Temperature is approximated with:

```c
temperatura = g_adc_temp * 0.4887;
```

## Digital Inputs

| Pin | Symbol | Function |
| --- | --- | --- |
| RB0 | `SW_IGNICAO` | Ignition switch |
| RB1 | `SW_PARTIDA` | Starter button |
| RB2 | `SW_MAPA` | Short press changes fuel map; long press changes loop mode |
| RB3 | `SW_MENU` | Cycles LCD display screens |

The inputs use active-low logic in the firmware, so a pressed/active switch is
read as `0`.

## Engine State

The firmware tracks three main operating states:

| State variable | Meaning |
| --- | --- |
| `g_ignicao_ligada` | Ignition switch is active |
| `g_modo_partida` | Starter button is pressed |
| `g_motor_girando` | Engine is considered running |

The engine is considered running when the starter is active and the simulated
RPM is above 300 RPM. If the engine is already running and RPM drops below 400
RPM, the firmware considers the engine stopped and disables Timer1 and all
injectors.

## Fuel Maps

The project contains two 16x16 fuel maps:

- `MAPA_ECONOMICO`: lower base injection values for economy mode;
- `MAPA_PERFORMANCE`: higher base injection values for performance mode.

Both maps contain 256 entries. The axes are simulated RPM and TPS. Since each
ADC reading is 10 bits, the firmware compresses each value into a 4-bit index:

```c
rpm_index = g_adc_rpm_sim >> 6;  // 0 to 15
tps_index = g_adc_tps >> 6;      // 0 to 15
index_linear = rpm_index * 16 + tps_index;
```

The selected map entry becomes the base injector pulse value:

```c
tempo_base = MAPA_ECONOMICO[index_linear];
// or
tempo_base = MAPA_PERFORMANCE[index_linear];
```

The active map is saved in EEPROM address `0` when changed while the engine is
not running. On startup, the firmware restores the saved map; invalid values
fall back to the economy map.

## Fuel Corrections

After reading the base value from the selected map, the firmware applies several
rules.

### Closed-Loop Correction

When closed-loop mode is enabled, the simulated lambda sensor is converted into
an AFR-like value. The target AFR is fixed at 14.7. A correction factor is
computed and clamped between 0.70 and 1.30 to avoid extreme corrections.

### RPM Limiter

If simulated RPM exceeds `RPM_LIMITE` (`6500`), the firmware sets the final
injection value to zero.

### Deceleration Fuel Cut

If TPS is near zero and RPM is above 1800, the firmware also sets the final
injection value to zero to simulate fuel cut during deceleration.

### Start Enrichment

When the starter is active, the final injection value is multiplied by 1.3 to
simulate a richer mixture during start.

### Timer2 Limit

The final value is capped at 255 because it is stored in an 8-bit Timer2 period
register value.

## Timer-Based Injection and Ignition

The firmware uses two timers:

| Timer | Purpose |
| --- | --- |
| Timer1 | Schedules engine events according to simulated RPM |
| Timer2 | Ends the injector pulse |

Timer1 is reloaded with a value calculated from the simulated RPM:

```c
ticks = 18750000 / g_rpm_real;
g_timer1_preload = 65535 - ticks;
```

On each Timer1 interrupt, the firmware advances a four-step sequence:

| Step | Ignition output | Injector opened |
| --- | --- | --- |
| 0 | `VELA_1_4` | `BICO_3` |
| 1 | `VELA_2_3` | `BICO_4` |
| 2 | `VELA_1_4` | `BICO_2` |
| 3 | `VELA_2_3` | `BICO_1` |

After opening an injector, Timer2 is started with the calculated pulse value.
When Timer2 overflows, all injectors are turned off.

## Auxiliary Actuators

| Output | Pin | Behavior |
| --- | --- | --- |
| Fuel pump | RB4 | Turns on while the engine is running; also primes briefly after ignition is switched on |
| Radiator fan | RB5 | Turns on above 95 C and turns off below 90 C |
| Check engine LED | RB6 | Turns on for TPS fault, high temperature, or RPM limiter condition |
| Starter motor | RB7 | Turns on while starter mode is active and the engine is not yet running |

The system shutdown routine clears all auxiliary outputs, ignition outputs,
injectors, and timers.

## LCD Interface

The LCD has three display modes:

| Mode | Displayed information |
| --- | --- |
| General | RPM, TPS, AFR, or a sport-style RPM bar depending on the active map |
| Map | Active fuel map and loop mode |
| Warnings | Temperature and system warnings |

The menu button cycles between these modes. The map button changes the fuel map
on a short press and toggles open-loop/closed-loop mode on a long press.

