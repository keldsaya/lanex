SECTIONS
{
  . = 0x10000; 
  kernel_start = .;
  .text : AT(ADDR(.text)) { *(.text) }
  .rodata : ALIGN(4K) { *(.rodata) }
  .data : ALIGN(4K) { *(.data) }
  .bss : ALIGN(4K) { *(.bss) *(COMMON) }
  . = ALIGN(4K);
  kernel_end = .;
}
