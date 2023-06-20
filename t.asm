
CPU 386

BITS 32

  extern ExitProcess

segment .text use32

run:
..start:
  push    ebp
  mov     ebp, esp

  mov     esp, ebp
  pop     ebp

  push    dword 123
  call    [ExitProcess]

segment .data use32

segment .bss use32
