@echo off
3rdparty\nasm\nasm -fobj test.asm -o test.obj
3rdparty\alink\alink test.obj -oPE -subsys console -o test.exe
if exist 3rdparty\verpatch\verpatch.exe if exist test.exe 3rdparty\verpatch\verpatch -va test.exe
