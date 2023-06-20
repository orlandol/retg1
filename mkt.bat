@echo off
nasm -fobj t.asm -o t.obj
alink t.obj 3rdparty\alink\win32.lib -oPE -subsys console -o t.exe
