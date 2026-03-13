
#ifndef RETG1_H
#define RETG1_H

#include <stdint.h>
#include <stdio.h>

/*
 *  Retineo String declarations
 */

#define RETSTR_PADBITS 8

#define RETSTR_PADMASK ((RETSTR_PADBITS) - 1)

#define RETSTR_PAD(size)\
  ((size + 1 + RETSTR_PADBITS) & (~(RETSTR_PADMASK)))

typedef char retstr;

retstr* retstrAllocate( uint32_t length );
retstr* retstrCopy( const retstr* rstring );
retstr* retstrCopyStr( const char* cstring );
void retstrRelease( retstr** retstrPtr );

uint32_t retstrLength( const retstr* retString );
uint32_t retstrTotalSize( const retstr* retString );

/*
 *  Program argument parser declarations
 */

typedef struct Options {
  retstr* sourceName;
  retstr* asmName;
  retstr* objName;
  retstr* binaryName;
} Options;

unsigned ParseOptions( int argc, char** argv, Options* options );

/*
 *  Symbol table declarations
 */

/*
 *  Source file declarations
 */

typedef struct Source {
  FILE* handle;
  uint32_t line;
  uint32_t column;
  char ch;
} Source;

Source* OpenSourceFile( const char* fileName );
void CloseSourceFile( Source** sourcePtr );

char ReadChar( Source* source );

/*
 *  Win32 x86 code generator declarations
 */

typedef struct AsmGen {
  FILE* handle;
} AsmGen;

AsmGen* CreateAsmFile( const char* fileName );
void CloseAsmFile( AsmGen** asmgenPtr );


unsigned EmitAsm( AsmGen* asmgen, const char* asmString );

/*
 *  Retineo expression parser declarations
 */

/*
 *  Retineo parser declarations
 */

unsigned NextToken( Source* source );

unsigned Match( Source* source, const char* withString );
unsigned Submatch( Source* source, const char* withString );

unsigned ParseProgram( Source* source, AsmGen* asmgen );

#endif
