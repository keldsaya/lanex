[org 0x7c00] ; where to load
bits 16 ; real mode

start:
  cli                 ; no interrupts

  xor ax, ax
  mov ds, ax          ; data seg 0
  mov es, ax          ; extra seg 0
  mov ss, ax          ; stack seg 0
  mov sp, 0x7c00      ; stack pointer here so no overwriting

  ; al to debug successful bootload
  mov ah, 0x0E
  mov al, 'B'         
  int 0x10

  mov [boot_drive], dl   

  call get_mem_info
  call enable_a20
  call load_kernel

  lgdt [gdt_descriptor]

  ; protected mode by turning on CR0 
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  ; jump to protected mode code and switch segment
  jmp 0x08:init_pm   

%include "src/memory.asm"
%include "src/disk.asm"
%include "src/gdt.asm"

bits 32 ; 32 bit mode
init_pm:
  ; set up data seg registers
  mov ax, 0x10        ; data seg selector
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax          ; set stack segment

  mov esp, 0x90000    ; set stack pointer somewhere safe

  push dword [mem_high]
  push dword [mem_low]

  jmp 0x10000         ; jump to kernel code

boot_drive: db 0 ; what drive booted from

; fill up the rest to finish boot sector
times 510 - ($ - $$) db 0
dw 0xAA55
