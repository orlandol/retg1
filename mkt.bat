@echo off
3rdparty\nasm\nasm -fobj test.asm -o t.obj
3rdparty\alink\alink test.obj -oPE -subsys console -o test.exe
