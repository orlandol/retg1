@echo off

if exist retg1.c goto retg1Exists
  echo retg1.c not found
  goto exitBuild
:retg1Exists

if exist retstr.c goto retstrExists
  echo retstr.c not found
  goto exitBuild
:retstrExists

if exist retargs.c goto retargsExists
  echo retargs.c not found
  goto exitBuild
:retargsExists

if exist avl_tree.? goto avltreeExists
  echo avl_tree.c and/or avl_tree.h not found
  goto exitBuild
:avltreeExists

if exist symtab.c goto symtabExists
  echo symtab.c not found
  goto exitBuild
:symtabExists

if exist source.c goto sourceExists
  echo source.c not found
  goto exitBuild
:sourceExists

if exist cgeni32.c goto cgeni32Exists
  echo cgeni32.c not found
  goto exitBuild
:cgeni32Exists

if exist expr.c goto exprExists
  echo expr.c not found
  goto exitBuild
:exprExists

if exist peexe.c goto peexeExists
  echo peexe.c not found
  goto exitBuild
:peexeExists

if exist parse.c goto parseExists
  echo parse.c not found
  goto exitBuild
:parseExists

tcc\tcc.exe retg1.c retstr.c retargs.c avl_tree.c symtab.c source.c cgeni32.c expr.c peexe.c parse.c

:exitBuild
