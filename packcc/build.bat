@echo off
packcc retg1.peg
tcc retg1.c
verpatch /va retg1.exe
retg1 retc.ret
