
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "retlib.h"

Parser* retFile = NULL;
Executable* exeFile = NULL;

unsigned ParseOptions( int runArgsCount, const char* runArgs[] ) {
  return 3;
}

void Cleanup( void ) {
  CloseSource( &retFile );
  CloseExecutable( &exeFile );
}

int main( int argc, char* argv[] ) {
  atexit( Cleanup );

  return 0;
}
