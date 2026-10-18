.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global g_pfnVectors
.global _sidata
.global _sdata
.global _edata

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word 0x20010000
    .word Reset_Handler

.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcc copy_word

    bl main

1:
    b 1b

copy_word:
    ldr r3, [r0]
    str r3, [r1]
    adds r0, r0, #4
    adds r1, r1, #4
    b copy_data
