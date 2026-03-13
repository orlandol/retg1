@echo off

if exist *.obj del *.obj
if exist retg1.exe del retg1.exe

tcc\tcc.exe retg1.c retstr.c retargs.c avl_tree.c symtab.c source.c cgeni32.c expr.c parse.c
