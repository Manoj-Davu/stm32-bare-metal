STM32F401 Potentiometer Controlled LED Brightness
=================================================

MCU
---
STM32F401CCU6

Features
--------
- Bare-metal C
- Direct register-level ADC configuration
- ADC1_IN0 on PA0
- 12-bit ADC conversion
- TIM2 PWM on PA5
- Potentiometer-controlled LED brightness
- No HAL / LL libraries

Hardware
--------
- STM32F401CCU6
- 10k potentiometer
- LED
- Current-limiting resistor
- ST-Link V2

Connections
-----------
Potentiometer:
- Outer pin → 3.3V
- Wiper → PA0
- Outer pin → GND

LED:
- PA5 → resistor → LED → GND

ADC
---
ADC1_IN0
PA0
12-bit result: 0–4095

PWM
---
TIM2_CH1
PA5
ARR = 199
PSC = 419
PWM frequency = 1 kHz

ADC-to-PWM mapping
------------------
CCR1 = ADC_value × ARR / 4095

ADC = 0    → 0% brightness
ADC ≈ 2048 → ≈50% brightness
ADC = 4095 → ≈100% brightness

Tools
-----
- arm-none-eabi-gcc
- gdb-multiarch
- OpenOCD
- ST-Link V2
- Ubuntu Linux
- KiCad/breadboard hardware setup

Learning Outcome
----------------
This project demonstrates direct register-level configuration of
GPIO, ADC, timer PWM, clock control, ADC polling, and peripheral
data mapping on STM32F401.
