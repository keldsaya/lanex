.section .text
.align 4
.global isr_wrapper
.global keyboard_wrapper
.global pit_wrapper
.global rtc_wrapper
.extern isr_handler
.extern keyboard_handler
.extern pit_handler
.extern rtc_handler

.macro INTERRUPT_WRAPPER name, target
.global \name
\name:
    pushal
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    call \target
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popal
    iret
.endm

INTERRUPT_WRAPPER isr_wrapper, isr_handler
INTERRUPT_WRAPPER keyboard_wrapper, keyboard_handler
INTERRUPT_WRAPPER pit_wrapper, pit_handler
INTERRUPT_WRAPPER rtc_wrapper, rtc_handler
