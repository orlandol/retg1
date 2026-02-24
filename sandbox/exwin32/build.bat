@echo off
nasm -fobj exwin32.asm
alink -oPE -subsys console exwin32.obj
