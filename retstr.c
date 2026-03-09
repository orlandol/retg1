#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "retg1.h"

typedef struct retstrImpl {
  uint32_t length;
  uint32_t size;
} retstrImpl;

typedef struct blankRetstrImpl {
  uint32_t length;
  uint32_t size;
  char     data[8];
} blankRetstrImpl;

static const blankRetstrImpl blankRetStringData = {
  0,
  8,
  ""
};

static const retstr* blankRetString =
  ((retstr*)&blankRetStringData) + sizeof(retstrImpl);

retstr* retstrCreate( uint32_t size ) {
  retstrImpl* newStringImpl = NULL;
  retstr* newString = NULL;
  uint32_t paddedSize = 0;

  paddedSize = RETSTR_PAD(size);
  if( paddedSize < size ) { return NULL; }

  newStringImpl = calloc(1, sizeof(retstrImpl) + paddedSize);
  if( newStringImpl ) {
    newStringImpl->size = paddedSize;
    newString = ((retstr*)newStringImpl) + sizeof(retstrImpl);
  }

  return newString;
}

retstr* retstrCopy( const retstr* retString ) {
  retstrImpl* newStringImpl = NULL;
  retstr* newString = NULL;
  const retstrImpl* sourceStringImpl = NULL;
  const retstr* sourceString;
  uint32_t sourceLength = 0;
  uint32_t sourceSize = 0;

  sourceString = retString ? retString : blankRetString;
  sourceStringImpl = (retstrImpl*)(sourceString - sizeof(retstrImpl));

  sourceLength = sourceStringImpl->length;
  sourceSize = sourceStringImpl->size;

  newStringImpl = calloc(1, sizeof(retstrImpl) + sourceSize);
  if( newStringImpl ) {
    newStringImpl->length = sourceLength;
    newStringImpl->size = sourceSize;

    newString = ((retstr*)newStringImpl) + sizeof(retstrImpl);
    strncpy( newString, sourceString, sourceLength );
  }

  return newString;
}

retstr* retstrCopyStr( const char* cstring ) {
  retstrImpl* newStringImpl = NULL;
  retstr* newString = NULL;
  const char* sourceString;
  size_t sourceLength = 0;
  uint32_t paddedSize = 0;

  sourceString = cstring ? cstring : "";

  sourceLength = strlen(cstring);
  paddedSize = RETSTR_PAD(sourceLength);
  if( paddedSize < sourceLength ) { return NULL; }

  newStringImpl = calloc(1, sizeof(retstrImpl) + paddedSize);
  if( newStringImpl ) {
    newStringImpl->length = sourceLength;
    newStringImpl->size = paddedSize;

    newString = ((retstr*)newStringImpl) + sizeof(retstrImpl);
    strncpy( newString, sourceString, sourceLength );
  }

  return newString;
}

void retstrRelease( retstr** retstrPtr ) {
  if( retstrPtr ) {
    if( (*retstrPtr) ) {
      free( ((*retstrPtr) - sizeof(retstrImpl)) );
      (*retstrPtr) = NULL;
    }
  }
}
