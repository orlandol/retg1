
#include <stdlib.h>
#include <string.h>

#include "retg1.h"

typedef struct retstrImpl {
  uint32_t length;
  uint32_t size;
} retstrImpl;

#define RETSTR_PADBITS 8

#define RETSTR_PAD(length)\
  ((length + 1 + RETSTR_PADBITS) & (~RETSTR_PADBITS))

retstr* retstrCreate( uint32_t length ) {
  retstrImpl* newStr = NULL;
  uint32_t paddedLength = 0;

  paddedLength = RETSTR_PAD(length);
  if( paddedLength < length ) { return NULL; }

  newStr = calloc(1, paddedLength);

  if( newStr ) {
    newStr->size = paddedLength;
    return ((retstr*)newStr) + sizeof(retstrImpl);
  }

  return NULL;
}

retstr* retstrCopy( retstr* retString ) {
  return NULL;
}

retstr* retstrCopyStr( const char* cstring ) {
  retstrImpl* newStr = NULL;
  uint32_t paddedLength = 0;
  size_t clength = 0;

  if( cstring == NULL ) { return NULL; }

  clength = strlen(cstring);

  paddedLength = RETSTR_PAD(clength);
  if( paddedLength < clength ) { return NULL; }

  newStr = calloc(1, paddedLength);

  if( newStr ) {
    strncpy( (((retstr*)newStr) + sizeof(retstrImpl)), cstring, clength );
    newStr->length = clength;
    newStr->size = paddedLength;
    return ((retstr*)newStr) + sizeof(retstrImpl);
  }

  return NULL;
}

void retstrRelease( retstr** retstrPtr ) {
  if( retstrPtr ) {
    if( (*retstrPtr) ) {
      free( ((*retstrPtr) - sizeof(retstrImpl)) );
      (*retstrPtr) = NULL;
    }
  }
}
