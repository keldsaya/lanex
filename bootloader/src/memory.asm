bits 16

mem_low: dd 0
mem_high: dd 0

get_mem_info:
  xor cx, cx
  xor dx, dx
  mov ax, 0xE801
  int 0x15
  jc .error

  mov [mem_low], ax
  mov [mem_high], bx
  ret

.error:
  mov al, 'M'
  jmp disk_err

enable_a20:
  in al, 0x64
  test al, 0x2
  jnz enable_a20
  mov al, 0xd1
  out 0x64, al
.wait:
  in al, 0x64
  test al, 0x2
  jnz .wait
  mov al, 0xdf
  out 0x60, al
  ret

