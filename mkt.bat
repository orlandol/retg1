@echo off
3rdparty\nasm\nasm -fobj t.asm -o t.obj
3rdparty\alink\alink t.obj -oPE -subsys console -o t.exe
