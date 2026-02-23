
#include <stdlib.h>
#include <stdio.h>

#include "retg1.h"

retstr* sourceName = NULL;
retstr* binaryName = NULL;

unsigned warningCount = 0;
unsigned errorCount = 0;

void PrintBanner() {
  printf( "\nRetineo G1 Win32 x86 Compiler - "
          "Copyright 1993-2026 Orlando LLanes\n\n" );
}

void PrintUsage() {
  printf( "  usage: retg1 source[.ret] [binary.exe]\n\n" );
  exit( 1 );
}

void PrintSummary( const char* generatedName,
                   unsigned parseWarnings, unsigned parseErrors ) {
  printf( "  Completed building '%s': %u warning(s); %u error(s)\n\n",
    generatedName, warningCount, errorCount );
}

void Cleanup() {
  retstrRelease( &sourceName );
  retstrRelease( &binaryName );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  PrintBanner();

  if( argc <= 1 ) {
    PrintUsage();
  }

  PrintSummary( binaryName, warningCount, errorCount );

  Cleanup();

  return 0;
}
