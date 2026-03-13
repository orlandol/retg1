
#include <stdlib.h>
#include <stdio.h>

#include "retg1.h"

AsmGen* CreateAsmFile( const char* fileName ) {
  AsmGen* newAsmGen = NULL;

  if( !(fileName && (*fileName)) ) { return NULL; }

  newAsmGen = calloc(1, sizeof(AsmGen));
  if( newAsmGen ) {

    newAsmGen->handle = fopen(fileName, "rb");
    if( newAsmGen->handle ) {
      EmitAsm( newAsmGen, "\nBITS 32\n\nsection .text use32\n\n" );

      return newAsmGen;
    }

    free( newAsmGen );
    newAsmGen = NULL;
  }

  return newAsmGen;
}

void CloseAsmFile( AsmGen** asmgenPtr ) {
  if( asmgenPtr ) {
    if( (*asmgenPtr) ) {
      fclose( (*asmgenPtr)->handle );
      (*asmgenPtr)->handle = NULL;

      free( (*asmgenPtr) );
      (*asmgenPtr) = NULL;
    }
  }
}

unsigned EmitAsm( AsmGen* asmgen, const char* asmString ) {
  if( !(asmgen && asmgen->handle) ) { return 1; }

  if( !(asmString && (*asmString)) ) { return 2; }

  fprintf( asmgen->handle, "%s", asmString );

  return 0;
}
