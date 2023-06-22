@echo off
REM Batch file as a shortcut to remove repetetive typing
3rdparty\ev\ev retg1.peg test.ret
call build.bat
retg1 test.ret test.exe > t.asm
