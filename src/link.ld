/*-----------------------------------------------------------
 * Bare-metal AArch64 Linker Script
 * For GICv3 interrupt example (EL1)
 *----------------------------------------------------------*/

ENTRY(_start)

/* Adjust to match your board or QEMU memory map */
MEMORY
{
    /* Example for QEMU virt machine */
    RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 128M
}

SECTIONS
{
    /*-----------------------------------------------------------
     * Exception Vector Table (must be 2KB-aligned)
     *----------------------------------------------------------*/
    .vectors ALIGN(2048) : 
    {
        KEEP(*(.vectors))
    } > RAM

    /*-----------------------------------------------------------
     * Code and read-only data
     *----------------------------------------------------------*/
    .text ALIGN(0x1000) : 
    {
        *(.text.startup)
        *(.text*)
        *(.rodata*)
    } > RAM

    /*-----------------------------------------------------------
     * Initialized data
     *----------------------------------------------------------*/
    .data ALIGN(0x1000) : 
    {
        *(.data*)
    } > RAM

    /*-----------------------------------------------------------
     * Uninitialized data (zeroed at startup)
     *----------------------------------------------------------*/
    .bss ALIGN(0x1000) (NOLOAD) :
    {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > RAM

    /*-----------------------------------------------------------
     * Stack
     *----------------------------------------------------------*/
    .stack ALIGN(16) (NOLOAD) :
    {
        _stack_start = .;
        . += 0x4000;    /* 16KB stack (adjust as needed) */
        _stack_top = .;
    } > RAM

    /*-----------------------------------------------------------
     * Provide useful symbols
     *----------------------------------------------------------*/
    PROVIDE(end = .);
}
