@echo off
call clean.bat

packcc retg1.peg
if exist retg1.c tcc retg1.c
