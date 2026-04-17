global _start
extern kmain

_start:
    mov esp, stack_top    
    call kmain
    cli
.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB 
stack_top:
