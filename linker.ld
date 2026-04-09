ENTRY(_start)

SECTIONS
{
    . = 0x10000;

    .text :
    {
      build/main.o(.text)
      *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }
}
