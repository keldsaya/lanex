bits 16

load_kernel:
  mov ax, 0x1000
  mov es, ax
  xor bx, bx
  mov ah, 0x02

  mov al, 128

  mov ch, 0
  mov dh, 0
  mov cl, 2 
  mov dl, [boot_drive]
  int 0x13
  jc disk_err
  ret

disk_err:
  mov ah, 0x0E
  mov al, 'E'
  int 0x1000
  jmp $

