
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "retg1.h"

Options options = {};

Source* source = NULL;
Binary* binary = NULL;

unsigned Execute( const char* command, const char* arguments[], unsigned* exitCode ) {
  unsigned result = 0;

  if( !(command && (*command)) ) { return 2; }
  if( !(arguments && (*arguments)) ) { return 3; }

  result = _spawnv(_P_WAIT, command, arguments);

  if( exitCode ) { *exitCode = result; }

  return 0;
}

void PrintBanner() {
  printf( "\nRetineo G1 Win32 x86 Compiler - "
          "Copyright 1993-2026 Orlando LLanes\n\n" );
}

void PrintUsage() {
  printf( "  usage: retg1 source[.ret] [binary.exe]\n\n" );
}

void PrintSummary( const char* binaryName ) {
  printf( "  Completed building '%s'\n\n", binaryName );
}

void Cleanup() {
  CloseSourceFile( &source );
  CloseBinaryFile( &binary );

  retstrRelease( &options.sourceName );
  retstrRelease( &options.asmName );
  retstrRelease( &options.objName );
  retstrRelease( &options.binaryName );
}

int main( int argc, char** argv ) {
  unsigned exitCode = 0;

  atexit( Cleanup );

  PrintBanner();

  if( argc <= 1 ) {
    PrintUsage();
    return 1;
  }

  ParseOptions( argc, argv, &options );

  ParseProgram( source, binary );

  //nasm -fobj binary.asm
  const char* nasmArguments[] = {
    "-fobj",
    options.asmName,
    NULL
  };
  //Execute( "nasm.exe", nasmArguments, &exitCode );
  //Error check

  //alink -oPE -subsys console binary.obj
  const char* alinkArguments[] = {
    "-oPE",
    "-subsys console",
    options.objName,
    NULL
  };
  //Execute( "alink.exe", alinkArguments, &exitCode );
  //Error check

  PrintSummary( options.binaryName );

  Cleanup();

  return 0;
}
