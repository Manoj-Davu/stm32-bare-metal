ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
    .text :
    {
        /* Vector table must be first in Flash */
        . = ALIGN(4);
        KEEP(*(.isr_vector))

        /* Program code */
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    _sidata = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        *(.bss*)
        *(COMMON)
    } > RAM
}
