
#ifndef RETG1_H
#define RETG1_H

#include <stdint.h>

/*
 *  Retineo String declarations
 */

#define RETSTR_PADBITS 8

#define RETSTR_PADMASK ((RETSTR_PADBITS) - 1)

#define RETSTR_PAD(size)\
  ((size + 1 + RETSTR_PADBITS) & (~(RETSTR_PADMASK)))

typedef char retstr;

retstr* retstrCreate( uint32_t length );
retstr* retstrCopy( const retstr* retString );
retstr* retstrCopyStr( const char* cstring );
void retstrRelease( retstr** retstrPtr );

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

/*
 *  Retineo expression parser declarations
 */

/*
 *  Retineo parser declarations
 */

/*
 *  Win32 x86 code generator declarations
 */

#endif
