@echo off
call clean.bat

3rdparty\packcc\packcc retg1.peg
if exist retg1.c 3rdparty\tcc\tcc retg1.c
