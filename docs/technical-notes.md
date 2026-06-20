# Technical Notes

## Academic Goal

The goal of this project was to apply microprocessor and microcontroller
concepts in an embedded system with multiple inputs and outputs, real-time
timing requirements, and interaction with simulated hardware.

## Firmware

The firmware is centered around `Software-MPLab/ecu_final.X/main.c` and uses:

- PIC16F877A configuration bits;
- ADC for sensor acquisition;
- Timer1 as the base timing source for the engine cycle;
- Timer2 for injector pulse duration;
- interrupts for injection and ignition timing events;
- internal EEPROM to store the active map;
- a 16x2 LCD for the local interface.

## Inputs

| Input | Function |
| --- | --- |
| AN0 | Simulated TPS |
| AN1 | Simulated RPM |
| AN2 | Simulated lambda/oxygen sensor |
| AN3 | Simulated temperature |
| RB0 | Ignition switch |
| RB1 | Starter button |
| RB2 | Map switch / loop mode |
| RB3 | Display menu |

## Outputs

| Output | Function |
| --- | --- |
| RB4 | Fuel pump |
| RB5 | Radiator fan |
| RB6 | Check engine LED |
| RB7 | Starter motor |
| RC0, RC1, RC2, RC5 | Injectors |
| RE0, RE1 | Ignition pairs |
| RD0-RD4, RD6 | 16x2 LCD |

## Control Logic

RPM and TPS values select an entry from a 16x16 lookup table. The selected
value defines the base injector pulse time. The ECU also applies additional
rules for engine start, RPM limiting, deceleration fuel cut, and lambda-based
correction in simulated closed-loop mode.

A more detailed explanation of the runtime behavior is available in
[ECU operation](ecu-operation.md).

## Hardware Documentation

The `docs/images` directory contains exported views used to document the
hardware portion of the project:

- `esquematico.png`: KiCad schematic with the PIC16F877A, simulated analog
  sensors, LCD, buttons, and actuator driver stages.
- `design-pcb.png`: PCB routing view showing the two-layer board layout.
- `frente-pcb.png`: front 3D render with the LCD, user controls, and silkscreen.
- `tras-pcb.png`: back 3D render with the microcontroller, connectors, crystal,
  potentiometer, and output driver circuitry.

## Learning Outcomes

- Peripheral configuration on an 8-bit microcontroller.
- Timer and interrupt-driven event handling.
- ADC acquisition and conversion of simulated physical quantities.
- Calibration map organization.
- Integration between firmware, simulation, and PCB design.
- Practical program memory constraints in small microcontrollers.

## Future Improvements

- Split the firmware into smaller modules.
- Replace floating-point calculations with fixed-point arithmetic.
- Add abstraction layers for the LCD and actuators.
- Add logic tests for maps and limit conditions.
- Add Proteus screenshots to complement the KiCad schematic and PCB renders.
- Review ERC/DRC results and document them in the repository.
