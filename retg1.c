
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "retg1.h"

Options options = {};

unsigned warningCount = 0;
unsigned errorCount = 0;

unsigned Execute( const char* command, const char* arguments ) {
  return 3;
}

void PrintBanner() {
  printf( "\nRetineo G1 Win32 x86 Compiler - "
          "Copyright 1993-2026 Orlando LLanes\n\n" );
}

void PrintUsage() {
  printf( "  usage: retg1 source[.ret] [binary.exe]\n\n" );
}

void PrintSummary( const char* generatedName,
                   unsigned ParseWarnings, unsigned parseErrors ) {
  printf( "  Completed building '%s': %u warning(s); %u error(s)\n\n",
    generatedName, warningCount, errorCount );
}

void Cleanup() {
  retstrRelease( &options.sourceName );
  retstrRelease( &options.asmName );
  retstrRelease( &options.objName );
  retstrRelease( &options.binaryName );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  PrintBanner();

  if( argc <= 1 ) {
    PrintUsage();
    return 1;
  }

  ParseOptions( argc, argv, &options );

  PrintSummary( options.binaryName, warningCount, errorCount );

  Cleanup();

  return 0;
}
