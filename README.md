# STM32F401 Bare-Metal GPIO Project

A minimal **bare-metal firmware project for the STM32F401CCU6 (ARM Cortex-M4)**, developed from scratch without using STM32 HAL or CubeMX.

The project demonstrates the complete low-level firmware flow: **startup code → vector table → linker script → register-level GPIO configuration → compilation/linking → flashing → debugging with GDB and OpenOCD**.

An external LED is connected to **GPIOA Pin 5 (PA5)** and is controlled directly through the STM32 GPIO registers.

---

## 🎯 Project Objective

The goal of this project is to understand what happens underneath a typical embedded application rather than relying on high-level libraries.

Instead of using:

```c
HAL_GPIO_TogglePin(...);
```

the GPIO peripheral is controlled directly by accessing the STM32 memory-mapped registers.

This project was also used to learn and verify:

* ARM Cortex-M startup sequence
* Vector table
* Reset handler
* Linker script
* Flash and RAM memory mapping
* Register-level GPIO programming
* RCC peripheral clock enable
* GDB debugging
* OpenOCD
* ST-Link SWD debugging

---

## 🧰 Hardware

| Component           | Description             |
| ------------------- | ----------------------- |
| Microcontroller     | STM32F401CCU6           |
| CPU                 | ARM Cortex-M4           |
| Programmer/Debugger | ST-Link V2              |
| LED                 | External LED            |
| Resistor            | 680 Ω / 1 kΩ            |
| Breadboard          | Used for LED connection |
| Jumper wires        | GPIO connections        |

### LED Connection

The external LED is connected to **PA5**:

```text
STM32F401CCU6

PA5 ─── 680Ω/1kΩ ───►|─── GND
                     LED
```

* LED anode → resistor → PA5
* LED cathode → GND
* A current-limiting resistor is required.

---

## 🏗️ Project Structure

```text
stm32f401-bare-metal/
│
├── src/
│   ├── main.c
│   └── startup.s
│
├── linker.ld
│
├── build/
│   ├── main.o
│   ├── startup.o
│   ├── blink.elf
│   └── blink.bin
│
└── README.md
```

> The `build/` directory contains generated files and can optionally be excluded from Git using `.gitignore`.

---

## ⚙️ How the Firmware Works

The firmware performs the following operations:

```text
Reset
  ↓
Vector Table
  ↓
Reset_Handler
  ↓
main()
  ↓
Enable GPIOA clock
  ↓
Configure PA5 as output
  ↓
Toggle PA5
  ↓
Delay
  ↓
Toggle PA5 again
  ↓
Repeat
```

---

## 🔌 RCC Configuration

Before accessing GPIOA, its peripheral clock must be enabled.

The firmware modifies:

```text
RCC_AHB1ENR
Address: 0x40023830
```

The following operation enables the GPIOA clock:

```c
RCC_AHB1ENR |= (1 << 0);
```

Bit 0 corresponds to the GPIOA peripheral clock.

During debugging, the register was verified using GDB:

```text
0x40023830: 0x00000001
```

---

## 📌 GPIOA Configuration

GPIOA is located at:

```text
GPIOA Base Address = 0x40020000
```

The GPIOA registers used in this project are:

| Register    |      Address | Purpose                     |
| ----------- | -----------: | --------------------------- |
| GPIOA_MODER | `0x40020000` | GPIO pin mode configuration |
| GPIOA_ODR   | `0x40020014` | GPIO output data            |

### Configuring PA5 as Output

Each GPIO pin uses two bits in `GPIOx_MODER`.

For PA5:

```text
Pin = 5
Bits = 5 × 2 = 10
```

First, the mode bits are cleared:

```c
GPIOA_MODER &= ~(3 << (5 * 2));
```

Then PA5 is configured as general-purpose output:

```c
GPIOA_MODER |= (1 << (5 * 2));
```

This produces:

```text
PA5 mode bits = 01
```

which corresponds to:

```text
General-purpose output
```

---

## 💡 LED Control

The LED is toggled by changing bit 5 of `GPIOA_ODR`:

```c
GPIOA_ODR ^= (1 << 5);
```

Since:

```text
1 << 5 = 0x20
```

the PA5 output bit changes between:

```text
0 → 1
1 → 0
```

During debugging, this was verified directly through GDB.

Before toggling:

```text
0x40020014: 0x00000000
```

After toggling:

```text
0x40020014: 0x00000020
```

This confirmed that the firmware was actually changing the hardware GPIO register.

---

# 🧠 Startup Code

The project uses a custom ARM startup file:

```text
src/startup.s
```

The startup code defines the Cortex-M vector table and reset handler.

The vector table contains:

```asm
g_pfnVectors:
    .word 0x20010000
    .word Reset_Handler
```

The first entry provides the initial **Main Stack Pointer (MSP)**.

The second entry provides the address of `Reset_Handler`.

The startup sequence is:

```text
Power-on Reset
      ↓
Load MSP
      ↓
Load Reset_Handler
      ↓
Reset_Handler
      ↓
main()
```

---

# 🗺️ Vector Table Verification

The vector table is located at:

```text
0x08000000
```

It was verified using GDB:

```gdb
x/2wx 0x08000000
```

Result:

```text
0x08000000:  0x20010000  0x08000009
```

Therefore:

```text
0x08000000 → Initial Stack Pointer
0x08000004 → Reset_Handler
```

The `Reset_Handler` symbol itself is located at:

```text
0x08000008
```

The vector table contains:

```text
0x08000009
```

because Cortex-M uses Thumb instructions and the least-significant bit indicates Thumb state.

---

# 🔗 Linker Script

The project uses a custom linker script:

```text
linker.ld
```

The memory layout defines:

```text
FLASH
Origin = 0x08000000
Size   = 256 KB

RAM
Origin = 0x20000000
Size   = 64 KB
```

The vector table is placed at the beginning of Flash, followed by the program code.

The resulting layout is:

```text
0x08000000
│
├── Vector Table
│
├── Reset_Handler
│
├── delay()
│
└── main()
```

---

# 🐛 Debugging

The firmware was debugged using:

* **GDB**
* **OpenOCD**
* **ST-Link V2**
* **SWD**

### Start OpenOCD

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
```

OpenOCD starts the GDB server on:

```text
localhost:3333
```

### Start GDB

```bash
gdb-multiarch build/blink.elf
```

Connect to the target:

```gdb
target extended-remote localhost:3333
```

Reset and halt:

```gdb
monitor reset halt
```

Load the firmware:

```gdb
load
```

Set a breakpoint:

```gdb
break main
```

Continue execution:

```gdb
continue
```

---

# 🔍 Register-Level Debugging

One of the main goals of this project was to verify that the C code was actually modifying the STM32 hardware registers.

For example:

### RCC

```gdb
x/wx 0x40023830
```

Result:

```text
0x40023830: 0x00000001
```

GPIOA clock enabled.

### GPIOA_MODER

```gdb
x/wx 0x40020000
```

Result:

```text
0xa8000400
```

PA5 configured as output.

### GPIOA_ODR

Before toggle:

```text
0x40020014: 0x00000000
```

After toggle:

```text
0x40020014: 0x00000020
```

This confirmed that PA5 was being driven HIGH.

---

# 🛠️ Build

The project uses the ARM GNU toolchain.

Compile the startup file:

```bash
arm-none-eabi-gcc \
-mcpu=cortex-m4 \
-mthumb \
-g \
-c src/startup.s \
-o build/startup.o
```

Compile the C source:

```bash
arm-none-eabi-gcc \
-mcpu=cortex-m4 \
-mthumb \
-g \
-c src/main.c \
-o build/main.o
```

Link the object files:

```bash
arm-none-eabi-gcc \
-mcpu=cortex-m4 \
-mthumb \
-T linker.ld \
-nostdlib \
-g \
build/startup.o \
build/main.o \
-o build/blink.elf
```

Generate the binary:

```bash
arm-none-eabi-objcopy \
-O binary \
build/blink.elf \
build/blink.bin
```

---

# 📤 Flashing

The ELF file can be loaded through GDB:

```gdb
target extended-remote localhost:3333
monitor reset halt
load
```

The firmware can then be executed:

```gdb
continue
```

---

# 📚 What I Learned

This project helped me understand the relationship between software and hardware at the register level.

### Software side

* C programming for embedded systems
* ARM Cortex-M startup
* Assembly
* Linker scripts
* ELF and binary files
* Memory mapping
* Vector tables
* Cross compilation

### Hardware side

* RCC peripheral clocks
* GPIO registers
* GPIO modes
* Output data registers
* Memory-mapped peripherals

### Debugging

* GDB
* OpenOCD
* ST-Link
* SWD
* Hardware breakpoints
* Inspecting MCU registers directly

---

# 🚀 Next Steps

This project is the starting point for a larger bare-metal STM32 learning path.

Planned topics:

* [ ] GPIO input
* [ ] External interrupts
* [ ] Timers
* [ ] SysTick
* [ ] UART
* [ ] SPI
* [ ] I²C
* [ ] ADC
* [ ] DMA
* [ ] Interrupt-driven drivers
* [ ] Embedded communication protocols
* [ ] FreeRTOS
* [ ] Larger bare-metal projects

---

## 📖 References

* STM32F401CCU6 Datasheet
* STM32F4 Reference Manual
* ARM Cortex-M4 Technical Reference Manual
* ARMv7-M Architecture Reference Manual

---

## 👨‍💻 Author

**Naga Manoj Davu**

This project is part of my journey into **bare-metal programming, firmware development, and embedded systems engineering**.
