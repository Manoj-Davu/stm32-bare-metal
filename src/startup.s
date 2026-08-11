.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global g_pfnVectors

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word 0x20010000
    .word Reset_Handler

.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    bl main

1:
    b 1b
