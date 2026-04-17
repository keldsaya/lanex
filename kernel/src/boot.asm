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

  call .wait_for_kbc
  mov al, 0xd1
  out 0x64, al
  call .wait_for_kbc
  mov al, 0xdf
  out 0x60, al
  call .wait_for_kbc

  mov ax, 0x1000
  mov es, ax
  xor bx, bx
  mov ah, 0x02
  mov al, 128          ; read sectors (val / 2 = MAX SIZE KERNEL.BIN IN KB)
  mov ch, 0
  mov dh, 0
  mov cl, 2           ; start from sector 2 (after boot sector)
  mov dl, [boot_drive]   
  int 0x13
  jc disk_error       ; if error, jump to handler

  ; GDT so CPU knows how to understand mem regions
  lgdt [gdt_descriptor]

  ; protected mode by turning on CR0 
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  ; jump to protected mode code and switch segment
  jmp 0x08:init_pm   

.wait_for_kbc:
  in al, 0x64
  test al, 0x2
  jnz .wait_for_kbc  
  ret

disk_error:           ; show E if error
  mov ah, 0x0E
  mov al, 'E'
  int 0x10
  jmp $

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

  jmp 0x10000         ; jump to kernel code

hang:
  jmp hang

; GDT tells CPU about memory parts to use
gdt_start:
  dq 0x0000000000000000                ; null descriptor
  dq 0x00CF9A000000FFFF                ; code segment for programs - 4GB limit
  dq 0x00CF92000000FFFF                ; data segment for storing data - 4GB limit
gdt_end:

gdt_descriptor:
  dw gdt_end - gdt_start - 1 ; size of GDT - 1
  dd gdt_start ; where GDT starts

boot_drive: db 0 ; what drive booted from

CODE_SEG equ 0x08 ; use code segment
DATA_SEG equ 0x10 ; use data segment

; fill up the rest to finish boot sector
times 510 - ($ - $$) db 0
dw 0xAA55
