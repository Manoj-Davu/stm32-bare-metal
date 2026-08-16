# STM32 Bare-Metal Traffic Light Simulator

A traffic light simulator implemented from scratch using bare-metal
programming on the STM32F401CCU6.

## Features

- Red LED control
- Yellow LED control
- Green LED control
- Push-button input
- Software delay
- GPIO register-level programming
- Traffic-light state sequencing

## Behavior

1. Red LED turns ON
2. After a delay, Red turns OFF
3. Yellow LED turns ON
4. After a delay, Yellow turns OFF
5. The sequence repeats
6. When the button is pressed, Green turns ON
7. After a delay, all three LEDs turn ON
8. The simulation stops

## Concepts Used

- STM32 GPIO
- RCC clock enable
- GPIO input/output configuration
- Register-level programming
- Button polling
- Timing and delays
- Embedded C

## Hardware

- STM32F401CCU6
- Red LED
- Yellow LED
- Green LED
- Push button
- Resistors
- Breadboard
- Jumper wires

## Toolchain

- ARM GCC
- OpenOCD
- GDB
- Ubuntu Linux

## Future Improvements

- Replace polling with EXTI interrupts
- Implement a finite state machine
- Replace blocking delays with a hardware timer
