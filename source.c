
#include <stdlib.h>
#include <stdio.h>

#include "retg1.h"

Source* OpenSourceFile( const char* fileName ) {
  Source* newSource = NULL;

  if( !(fileName && (*fileName)) ) { return NULL; }

  newSource = calloc(1, sizeof(Source));
  if( newSource ) {

    newSource->handle = fopen(fileName, "rb");
    if( newSource->handle ) {
      return newSource;
    }

    free( newSource );
    newSource = NULL;
  }

  return newSource;
}

void CloseSourceFile( Source** sourcePtr ) {
  if( sourcePtr ) {
    if( (*sourcePtr) ) {
      fclose( (*sourcePtr)->handle );
      (*sourcePtr)->handle = NULL;

      free( (*sourcePtr) );
      (*sourcePtr) = NULL;
    }
  }
}

char ReadChar( Source* source ) {
  return 0;
}
