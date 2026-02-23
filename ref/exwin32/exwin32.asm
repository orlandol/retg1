;
;
; Self-contained NASM Win32 program example
;
;
; Based on information from the following article by Tought Craft
;
; https://bitcodersblog.wordpress.com/2017/05/10/win32-in-nasm-part-1/
;
;
; Build as follows:
; nasm -fobj exwin32.asm
; alink -oPE -subsys console exwin32.obj
;
;
; Test as follows (result should be 42):
; exwin32
; echo %errorlevel%
;
;
; Section order implied from NASM (Netwide Assembler) documentation
;
;

BITS 32

section .text use32

_f1:
  mov     eax, 42
  ret

..start:
  call    _f1
  push    eax
  call    [ExitProcess]

section .data use32

section .bss use32

extern ExitProcess
import ExitProcess kernel32.dll
